#pragma once
#include "Core.h"
#include "Transform.h"

namespace Core
{
	class WorldGenerator;
	class Feature;


	class LightNode
	{
	public:
		glm::vec3 Color;
		float Radius;
		bool CastsShadow;
		Transform Transform;
	};

	class Collider
	{
	public:
		enum class Type { Box };

		Core::Transform Transform;
		Type ShapeType;
	};

	class Node
	{
	public:
		std::string Tag;
		Transform Transform;
	};

	class Tile
	{
	protected:
		Scene* ScenePtr;
		WorldGenerator* Generator;
		std::string Name;
		Tile* Parent;

		// Tile contents
		std::vector<Node*> Transitions;
		std::vector<Node*> Features;
		std::vector<Node*> Placeables;
		std::vector<Node*> Creatures;
		std::vector<LightNode*> Lights;
		std::vector<Node*> AudioSources;
		std::vector<std::pair<std::string, std::string>> Meshes;
		std::vector<Collider*> Colliders;

		glm::vec3 LowerBound;
		glm::vec3 UpperBound;

		// Tile variables
		std::vector<Entity*> Entities;
		int TileWeight;
		int TileAdjustment;
		bool OnlySpawnOnce;

		std::unordered_map<Node*, Tile*> UsedTransitions;
		int Distance; 

	public:
		Transform Transform;

		Tile(std::string name);
		Tile(Scene* scene, WorldGenerator* generator, Tile* parent, Node* n, std::string name, int distance);
		virtual ~Tile();

		void LoadDefaults();
		void Load();
		void Save();

		void UpdateDistance(int distance, Tile* parent);
		Tile* FindClosest(glm::vec3 position);
		bool ContainsPosition(glm::vec3 position);

		void Destroy();

		void BuildContents();
		void DestroyContents();
		void BuildNeighbors();
		void DestroyNeighbors();

		void AddEntity(Entity* e);

		void GetDistantEntities(std::unordered_set<Entity*>* result, int maxTileDepth);
	};

}