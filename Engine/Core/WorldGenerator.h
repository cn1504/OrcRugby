#pragma once
#include "Core.h"
#include "Tile.h"

namespace Core
{
	class Feature;

	class Theme
	{
	public:
		Theme();
		~Theme();

		glm::vec3 AmbientLight;
		glm::vec3 LightColor;
	};

	class WorldGenerator
	{
	protected:
		Window* WindowPtr;

		Theme* OriginalTheme;
		Theme* NextTheme;
		float ThemeTransitionTimeElapsed;
		float ThemeTransitionDuration;

		int TileDepth;
		Tile* TileGraph;

		std::unordered_map<std::string, std::vector<std::string>> TilesByTransition;		// Tile transition category, Tiles with matching transition
		std::unordered_map<std::string, int> TileWeights;
		std::unordered_map<std::string, int> OriginalTileWeights;
		std::unordered_map<std::string, int> TileWeightAdjustments;

		std::unordered_map<std::string, std::vector<Feature*>> FeaturesByType;	
		std::unordered_map<Feature*, int> FeatureWeights;
		std::unordered_map<Feature*, int> OriginalFeatureWeights;
		std::unordered_map<Feature*, int> FeatureWeightAdjustments;

		float TimeElapsedSinceUpdate;
		float UpdateTickDuration;

		void ReadFeatureDirectory(std::string path);
		void ReadTileDirectory(std::string path);

	public:
		Theme* CurrentTheme;

		WorldGenerator(Window* window, std::string firstTile, int tileDepth = 1);
		~WorldGenerator();

		void BuildFeatureDatabase();

		void LoadTileDatabase();
		void BuildTileDatabase(); 
		void SetSpawnWeights(std::string tile, int currentWeight, int weight, int adjustment);

		void Update();
		int GetMaxTileDepth();

		std::string PickNextTile(std::string transitionType);
		void BuildFeature(Tile* owner, Node* spawningNode);

		void ThemeTransition(Theme* nextTheme, float duration);
	};

}