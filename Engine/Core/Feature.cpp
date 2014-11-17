#include "Feature.h"
#include "Entity.h"
#include "Assets.h"
#include "LightSource.h"

namespace Core
{

	Feature::Feature(std::string name)
	{
		Name = name;
	}

	Feature::~Feature() 
	{
		for (auto t : Placeables)
			delete t;
		for (auto t : Creatures)
			delete t;
		for (auto t : Lights)
			delete t;
		for (auto t : AudioSources)
			delete t;
		for (auto t : Colliders)
			delete t;
	}

	std::string Feature::GetName() { return Name; }
	std::string Feature::GetTag() { return Tag; }
	int Feature::GetSpawnWeight() { return SpawnWeight; }
	int Feature::GetSpawnWeightAdjustment() { return SpawnWeightAdjustment; }

	void Feature::LoadDefaultValues()
	{
		SpawnWeight = 100;
		SpawnWeightAdjustment = 100;

		Tag = "Building_Width1_Base";

		/*
		Meshes.push_back(std::pair<std::string, std::string>("Bench", "Wood"));
		Meshes.push_back(std::pair<std::string, std::string>("Bricks", "Brick"));
		Meshes.push_back(std::pair<std::string, std::string>("Ceiling", "Concrete"));
		Meshes.push_back(std::pair<std::string, std::string>("Crown_Molding", "Concrete"));
		Meshes.push_back(std::pair<std::string, std::string>("Door_Frame", "Wood"));
		Meshes.push_back(std::pair<std::string, std::string>("Door_Handle", "Steel"));
		Meshes.push_back(std::pair<std::string, std::string>("Ground", "Concrete"));
		Meshes.push_back(std::pair<std::string, std::string>("Sign", "Silver"));
		Meshes.push_back(std::pair<std::string, std::string>("Sign_Bottom", "Paint_White"));
		Meshes.push_back(std::pair<std::string, std::string>("Sign_Support", "Steel"));
		Meshes.push_back(std::pair<std::string, std::string>("Sign_Top", "Paint_White"));
		Meshes.push_back(std::pair<std::string, std::string>("Wall_Left", "Concrete"));
		Meshes.push_back(std::pair<std::string, std::string>("Wall_Right", "Concrete"));
		Meshes.push_back(std::pair<std::string, std::string>("Window_Frames", "Wood"));
		Meshes.push_back(std::pair<std::string, std::string>("Window_Panes", "Glass"));
		Meshes.push_back(std::pair<std::string, std::string>("Wood_Decoration", "Wood"));
		*/

		Save();
	}

	void Feature::Load()
	{
		// Load feature nodes

		if (Settings::Misc::VerboseLogging)
			Debug::Log("\t" + Name);

		std::ifstream file("Resources/Features/" + Name);
		if (!file)
		{
			Debug::Error("Invalid feature: " + Name);
		}

		auto size = Placeables.size();
		auto ss = Placeables.size();

		file.read((char*)&size, sizeof(size));
		Tag.resize(size);
		file.read(&Tag[0], size);

		file.read((char*)&SpawnWeight, sizeof(SpawnWeight));
		file.read((char*)&SpawnWeightAdjustment, sizeof(SpawnWeightAdjustment));
		
		file.read((char*)&size, sizeof(size));
		for (unsigned int i = 0; i < size; i++)
		{
			auto n = new Node;
			file.read((char*)&ss, sizeof(ss));
			n->Tag.resize(ss);
			file.read(&n->Tag[0], ss);

			file.read((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.read((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.read((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
			Placeables.push_back(n);
		}

		file.read((char*)&size, sizeof(size));
		for (unsigned int i = 0; i < size; i++)
		{
			auto n = new Node;
			file.read((char*)&ss, sizeof(ss));
			n->Tag.resize(ss);
			file.read(&n->Tag[0], ss);

			file.read((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.read((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.read((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
			Creatures.push_back(n);
		}

		// Lights
		file.read((char*)&size, sizeof(size));
		for (unsigned int i = 0; i < size; i++)
		{
			auto n = new LightNode;
			file.read((char*)&n->Color, sizeof(n->Color));
			file.read((char*)&n->Radius, sizeof(n->Radius));
			file.read((char*)&n->CastsShadow, sizeof(n->CastsShadow));
			file.read((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.read((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.read((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
			Lights.push_back(n);
		}

		//AudioSources;

		file.read((char*)&size, sizeof(size));
		for (unsigned int i = 0; i < size; i++)
		{
			std::string mesh, mat;
			
			file.read((char*)&ss, sizeof(ss));
			mesh.resize(ss);
			file.read(&mesh[0], ss);

			file.read((char*)&ss, sizeof(ss));
			mat.resize(ss);
			file.read(&mat[0], ss);

			Meshes.push_back(std::pair<std::string, std::string>(mesh, mat));
		}

		//colliders

		file.close();
	}

	void Feature::Save()
	{
		if (Settings::Misc::VerboseLogging)
			Debug::Log("Saving feature: " + Name);

		std::ofstream file("Resources/Features/" + Name);
		if (!file)
		{
			Debug::Error("Invalid feature: " + Name);
		}

		auto size = Tag.size();

		file.write((char*)&size, sizeof(size));
		file.write((char*)&Tag[0], size);
		file.write((char*)&SpawnWeight, sizeof(SpawnWeight));
		file.write((char*)&SpawnWeightAdjustment, sizeof(SpawnWeightAdjustment));
		
		size = Placeables.size();
		file.write((char*)&size, sizeof(size));
		for (auto n : Placeables)
		{
			size = n->Tag.size();
			file.write((char*)&size, sizeof(size));
			file.write((char*)&n->Tag[0], size);
			file.write((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.write((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.write((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
		}

		size = Creatures.size();
		file.write((char*)&size, sizeof(size));
		for (auto n : Creatures)
		{
			size = n->Tag.size();
			file.write((char*)&size, sizeof(size));
			file.write((char*)&n->Tag[0], size);
			file.write((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.write((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.write((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
		}

		// Lights
		size = Lights.size();
		file.write((char*)&size, sizeof(size));
		for (auto n : Lights)
		{
			file.write((char*)&n->Color, sizeof(n->Color));
			file.write((char*)&n->Radius, sizeof(n->Radius));
			file.write((char*)&n->CastsShadow, sizeof(n->CastsShadow));
			file.write((char*)&n->Transform.Position, sizeof(n->Transform.Position));
			file.write((char*)&n->Transform.Rotation, sizeof(n->Transform.Rotation));
			file.write((char*)&n->Transform.Scale, sizeof(n->Transform.Scale));
		}

		//AudioSources;

		size = Meshes.size();
		file.write((char*)&size, sizeof(size));
		for (auto n : Meshes)
		{
			size = n.first.size();
			file.write((char*)&size, sizeof(size));
			file.write((char*)&n.first[0], size);
			size = n.second.size();
			file.write((char*)&size, sizeof(size));
			file.write((char*)&n.second[0], size);
		}

		//colliders

		file.close();
	}

	void Feature::BuildContents(Scene* scene, Tile* owner, Node* spawningNode)
	{
		if (Settings::Misc::VerboseLogging)
			Debug::Log("\t" + Name);


		//if (Settings::Misc::VerboseLogging)
		//	Debug::Log("Lights ...");
		for (auto n : Lights)
		{
			//Debug::Log("\t(" + std::to_string(n->Transform.Position.x) + ", " + std::to_string(n->Transform.Position.y) + ", " + std::to_string(n->Transform.Position.z) + ") => " + std::to_string((int)(n->Color.x * 256.0f)) + ", " + std::to_string((int)(n->Color.y * 256.0f)) + ", " + std::to_string((int)(n->Color.z * 256.0f)));
			Entity* l = new Entity();
			l->Transform.Position = owner->Transform.Position + spawningNode->Transform.Position + spawningNode->Transform.Rotation * n->Transform.Position;
			l->Transform.Rotation = spawningNode->Transform.Rotation * n->Transform.Rotation;
			l->Transform.Scale = n->Transform.Scale;
			l->AddComponent(new LightSource(scene, n->Color, n->Radius, n->CastsShadow));
			l->AddComponent(Assets::Meshes["Sphere"]);
			l->AddComponent(Assets::Materials["Light"]);
			owner->AddEntity(l);
		}


		// Generate feature entities
		for (auto n : Meshes)
		{			
			auto e = new Entity();
						
			e->Transform.Position = glm::vec3(owner->Transform.ToMatrix() * spawningNode->Transform.ToMatrix() * glm::vec4(0.0f,0.0f,0.0f,1.0f));
			e->Transform.Rotation = spawningNode->Transform.Rotation * owner->Transform.Rotation;

			e->AddComponent(Assets::LoadMesh(n.first));
			e->AddComponent(Assets::Materials[(Assets::Materials.count(n.second) > 0) ? n.second : "Gold"]);
			owner->AddEntity(e);
		}
	}

}