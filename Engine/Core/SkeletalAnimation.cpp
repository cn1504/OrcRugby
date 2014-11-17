#include "SkeletalAnimation.h"
#include "Assets.h"

namespace Core
{
	// rm: ScreenTone
	
	SkeletalAnimation::SkeletalAnimation(std::string skeletonFilename, std::string animationFilename)
	{
		Skeleton = nullptr;
		SkeletonFilename = skeletonFilename;
		AnimationFilename = animationFilename;

		LoadFiles();

		AddAnimation("Idle", new Animation(0, 0));
	}


	SkeletalAnimation::~SkeletalAnimation()
	{
		if (Skeleton != nullptr)
			delete Skeleton;

		for (auto anim : Animations)
		{
			delete anim.second;
		}

		for (auto v : FrameEvents)
		{
			for (auto e : v.second)
			{
				delete e;
			}
			v.second.clear();
		}
		FrameEvents.clear();
	}


	void SkeletalAnimation::LoadFiles()
	{
		if (Skeleton != nullptr)
			delete Skeleton;
		
		for (auto v : FrameEvents)
		{
			for (auto e : v.second)
			{
				delete e;
			}
			v.second.clear();
		}
		FrameEvents.clear();

		std::ifstream skeletonFile;
		skeletonFile.open("Resources/Meshes/" + SkeletonFilename + ".skeleton", std::ios::binary);

		std::ifstream animationFile;
		animationFile.open("Resources/Meshes/" + AnimationFilename + ".animation", std::ios::binary);

		if (Settings::Misc::VerboseLogging)
		{
			Debug::Log("Loading Skeletal Animation: " + SkeletonFilename + ", " + AnimationFilename);
		}

		// Load animation file header
		animationFile.read((char*)&FrameCount, sizeof(FrameCount));
		animationFile.read((char*)&Duration, sizeof(Duration));
		animationFile.read((char*)&AnimatedBoneCount, sizeof(AnimatedBoneCount));

		// Load skeleton
		Skeleton = new Bone(skeletonFile, BoneIndex);

		// Load animation
		for (unsigned int i = 0; i < AnimatedBoneCount; i++)
		{
			unsigned int index;
			animationFile.read((char*)&index, sizeof(index));
			BoneIndex[index]->LoadFrames(animationFile);
		}

		skeletonFile.close();
		animationFile.close();
	}


	void SkeletalAnimation::Load()
	{
		AnimationTimeElapsed = 0.0f;
		CurrentFrame = 0;
		TimePerFrame = (float)(Duration / FrameCount);
		AnimationTimeScale = 1.0f;
		FrameCount++;

		PlayAnimation("Idle", true);
	}


	void SkeletalAnimation::Update()
	{
		AnimationTimeElapsed += Time::Delta;
		while (AnimationTimeElapsed > TimePerFrame / AnimationTimeScale)
		{
			CurrentFrame = NextFrame;			

			if (CurrentFrame == CurrentAnimation->LastFrame)
			{
				if (isLooping)
				{
					NextFrame = CurrentAnimation->FirstFrame;
				}
				else
					PlayAnimation("Idle", true);
			}
			else
			{
				NextFrame++;
			}
			AnimationTimeElapsed -= TimePerFrame / AnimationTimeScale;

			if (FrameEvents.count(CurrentFrame) > 0)
				for (auto te : FrameEvents[CurrentFrame])
				{
					te->Run(Entity);
				}
		}

		Skeleton->UpdateTransform(CurrentFrame, NextFrame, AnimationTimeElapsed / (TimePerFrame / AnimationTimeScale), glm::mat4());
	}

	
	float SkeletalAnimation::GetAnimationTime(std::string name)
	{
		if (Animations.count(name) > 0)
			return (Animations[name]->LastFrame - Animations[name]->FirstFrame + 2) * (TimePerFrame / AnimationTimeScale);
		return 0;
	}


	void SkeletalAnimation::EnableBuffers(Shader* shader)
	{
	}


	void SkeletalAnimation::Render(Shader* shader)
	{
		//Core::Debug::Log("Rendering skeleton. " + std::to_string(BoneIndex.size()));

		// Set bone transforms
		for (auto bone : BoneIndex)
		{
			auto loc = shader->GetUL("BoneTransform[" + std::to_string(bone.first) + "]");
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(bone.second->CurrentTransform));
		}

		for (auto pair : Meshes)
		{
			// Set Material
			auto mat = (Assets::Materials.count(pair.second) > 0) ? Assets::Materials[pair.second] : Assets::Materials["Gold"];
			auto mesh = Assets::Meshes[pair.first];

			mat->WriteShaderUniforms(shader);

			mesh->EnableBuffers(shader);
			mesh->Render(shader);
			mesh->DisableBuffers(shader);
		}
	}


	void SkeletalAnimation::DisableBuffers(Shader* shader)
	{
	}


	void SkeletalAnimation::AddMesh(std::string mesh, std::string mat)
	{
		Meshes.push_back(std::pair<std::string, std::string>(mesh, mat));
	}
	

	void SkeletalAnimation::AddAnimation(std::string name, Animation* anim)
	{
		auto c = Animations.count(name);
		if (c > 0)
		{
			delete Animations[name];
		}
		Animations[name] = anim;
	}


	void SkeletalAnimation::AddFrameEvent(unsigned int frame, TriggerEvent* te)
	{
		FrameEvents[frame].push_back(te);
	}


	void SkeletalAnimation::PlayAnimation(std::string name, bool loop)
	{
		if (Animations.count(name) > 0 && CurrentAnimation != Animations[name])
		{
			CurrentAnimation = Animations[name];
			NextFrame = CurrentAnimation->FirstFrame;
			AnimationTimeScale = CurrentAnimation->TimeScale;
			isLooping = loop;
		}
	}

	
	void SkeletalAnimation::LoadSkeleton(std::string skeletonFilename)
	{
		SkeletonFilename = skeletonFilename;
		LoadFiles();
		Load();
	}


	void SkeletalAnimation::LoadAnimation(std::string animationFilename)
	{
		AnimationFilename = animationFilename;
		LoadFiles();
		Load();
	}


	std::string SkeletalAnimation::GetSkeletonFilename()
	{
		return SkeletonFilename;
	}


	std::string SkeletalAnimation::GetAnimationFilename()
	{
		return AnimationFilename;
	}


	void SkeletalAnimation::Save(std::ostream& out)
	{
		out << SkeletonFilename << std::endl;
		out << AnimationFilename << std::endl;

		out << Meshes.size() << std::endl;
		for (int i = 0; i < Meshes.size(); i++)
		{
			out << Meshes[i].first << std::endl;
			out << Meshes[i].second << std::endl;
		}

		out << Animations.size() << std::endl;
		for (auto a : Animations)
		{
			out << a.first << std::endl;
			out << a.second->FirstFrame << std::endl;
			out << a.second->LastFrame << std::endl;
			out << a.second->TimeScale << std::endl;
		}


		int count = 0;
		for (auto v : FrameEvents)
		{
			for (auto e : v.second)
			{
				count++;
			}
		}
		out << count << std::endl;
		for (auto v : FrameEvents)
		{
			for (auto e : v.second)
			{
				out << v.first << std::endl;
				e->Save(out);
			}
		}
	}


	unsigned int SkeletalAnimation::GetFrameCount()
	{
		return FrameCount;
	}



	Animation::Animation(unsigned int first, unsigned int last, float timeScale)
	{
		FirstFrame = first;
		LastFrame = last;
		TimeScale = timeScale;
	}


	Animation::~Animation()
	{
	}


	Bone::Bone(std::istream& file, std::unordered_map<unsigned int, Bone*>& BoneIndex)
	{
		float transform[10];
		unsigned int childCount;
		Transform* t;

		file.read((char*)&Index, sizeof(Index));
		file.read((char*)transform, 10 * sizeof(float));
		file.read((char*)&childCount, sizeof(childCount));

		t = new Transform(transform);
		Frames.push_back(t);
		
		BoneIndex[Index] = this;

		for (unsigned int i = 0; i < childCount; i++)
		{
			Children.push_back(new Bone(file, BoneIndex));
		}
	}


	Bone::~Bone()
	{
		for (auto i : Children)
			delete i;
		for (auto i : Frames)
			delete i;
	}

	
	void Bone::LoadFrames(std::istream& animationFile)
	{
		unsigned int count;
		animationFile.read((char*)&count, sizeof(count));

		for (unsigned int i = 0; i < count; i++)
		{
			Transform* frame = new Transform;
			animationFile.read((char*)&frame->Position, sizeof(frame->Position));
			animationFile.read((char*)&frame->Rotation, sizeof(frame->Rotation));
			animationFile.read((char*)&frame->Scale, sizeof(frame->Scale));
			Frames.push_back(frame);
		}
	}


	void Bone::UpdateTransform(int frame1, int frame2, float time, const glm::mat4& parentTransform)
	{

		//Core::Debug::Log("Updating skeleton. " + std::to_string(Index) + " " + std::to_string(Frames.size()));

		auto f1 = (frame1 < Frames.size()) ? Frames[frame1] : Frames[0];
		auto f2 = (frame2 < Frames.size()) ? Frames[frame2] : Frames[0];
		Transform t;
		t.Position = (1.0f - time) * f1->Position + time * f2->Position;
		t.Scale = (1.0f - time) * f1->Scale + time * f2->Scale;
		t.Rotation = glm::mix(f1->Rotation, f2->Rotation, time);
		CurrentTransform = parentTransform * t.ToMatrix();

		for (auto i : Children)
		{
			i->UpdateTransform(frame1, frame2, time, CurrentTransform);
		}
	}

}