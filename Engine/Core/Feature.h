#pragma once
#include "Core.h"
#include "Transform.h"
#include "WorldGenerator.h"
#include "Tile.h"

namespace Core
{
	
	class Feature
	{
	protected:
		std::string Name;
		std::string Tag;
		
		int SpawnWeight;
		int SpawnWeightAdjustment;

		std::vector<Node*> Placeables;
		std::vector<Node*> Creatures;
		std::vector<LightNode*> Lights;
		std::vector<Node*> AudioSources;
		std::vector<std::pair<std::string, std::string>> Meshes;
		std::vector<Collider*> Colliders;

	public:
		Feature(std::string name);
		~Feature();

		std::string GetName();
		std::string GetTag();
		int GetSpawnWeight();
		int GetSpawnWeightAdjustment();

		void LoadDefaultValues();
		void Load();
		void Save();

		void BuildContents(Scene* scene, Tile* owner, Node* spawningNode);
	};

}