#pragma once
#include <Core.h>
#include <Window/Renderer/Renderable.h>
#include <Assets/Mesh.h>
#include <Assets/Material.h>
#include <Window/Renderer/Shader.h>
#include "TriggerEvent.h"

namespace Core
{
	namespace Components
	{

		class Bone
		{
		protected:
			unsigned int Index;
			std::vector<Bone*> Children;
			std::vector<Transform*> Frames;

		public:
			glm::mat4 CurrentTransform;

			Bone(std::istream& file, std::unordered_map<unsigned int, Bone*>& BoneIndex);
			virtual ~Bone();

			void LoadFrames(std::istream& animationFile);
			void UpdateTransform(int frame1, int frame2, float time, const glm::mat4& parentTransform);
		};

		class Animation
		{
		public:
			Animation(unsigned int first, unsigned int last, float timeScale = 1.0f);
			~Animation();

			unsigned int FirstFrame;
			unsigned int LastFrame;
			float TimeScale;
		};

		class SkeletalAnimation
			: public Renderable
		{
		protected:
			std::string SkeletonFilename;
			std::string AnimationFilename;
			void LoadFiles();

			Bone* Skeleton;
			std::unordered_map<unsigned int, Bone*> BoneIndex;

			unsigned int FrameCount;
			double Duration;
			unsigned int AnimatedBoneCount;
			float TimePerFrame;

			float AnimationTimeElapsed;
			float AnimationTimeScale;
			unsigned int CurrentFrame;
			unsigned int NextFrame;
			Animation* CurrentAnimation;
			bool isLooping;

		public:
			std::vector<std::pair<std::string, std::string>> Meshes;
			std::unordered_map<std::string, Animation*> Animations;
			std::unordered_map<unsigned int, std::vector<TriggerEvent*>> FrameEvents;

			SkeletalAnimation(std::string skeletonFilename, std::string animationFilename);
			virtual ~SkeletalAnimation();
			virtual void Load();
			virtual void Update();

			virtual void EnableBuffers(Shader* shader);
			virtual void Render(Shader* shader);
			virtual void DisableBuffers(Shader* shader);

			void AddMesh(std::string mesh, std::string mat);
			void AddAnimation(std::string name, Animation* anim);
			void AddFrameEvent(unsigned int frame, TriggerEvent* te);
			void PlayAnimation(std::string name, bool loop = false);
			float GetAnimationTime(std::string name);

			void LoadSkeleton(std::string skeletonFilename);
			void LoadAnimation(std::string animationFilename);
			std::string GetSkeletonFilename();
			std::string GetAnimationFilename();
			void Save(std::ostream& out);

			unsigned int GetFrameCount();
		};

	}
}