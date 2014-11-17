#include "WorldGenerator.h"
#include "Window.h"
#include "Input.h"
#include "Feature.h"

namespace Core
{

	WorldGenerator::WorldGenerator(Window* window, std::string firstTile, int tileDepth)
	{
		WindowPtr = window;

		TileDepth = tileDepth;
		UpdateTickDuration = 1.0f;

		// Load Default Theme
		if (Settings::Misc::VerboseLogging)
		{
			Debug::Log("");
			Debug::Log("Tolerating Themes ...");
		}
		OriginalTheme = nullptr;
		NextTheme = nullptr;
		ThemeTransitionTimeElapsed = 0.0f;
		ThemeTransitionDuration = -1.0f;

		CurrentTheme = new Theme;
		CurrentTheme->AmbientLight = glm::vec3(0.2f);
		CurrentTheme->LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		WindowPtr->Scene->Theme = CurrentTheme;

		// Build generator database
		if (Settings::Misc::VerboseLogging)
		{
			Debug::Log("");
			Debug::Log("Building Features ...");
		}
		BuildFeatureDatabase();
		Debug::Log("\t" + std::to_string(FeatureWeights.size()) + " features loaded.");

		if (Settings::Misc::VerboseLogging)
		{
			Debug::Log("");
			Debug::Log("Scraping Tiles ...");
		}
		BuildTileDatabase();
		// LoadTileDatabase();	// Build or Load (not both)
		Debug::Log("\t" + std::to_string(TileWeights.size()) + " tiles loaded.");

		// Spawn starting tiles
		if (Settings::Misc::VerboseLogging)
		{
			Debug::Log("");
			Debug::Log("Generating World ...");
		}
		TileGraph = new Tile(WindowPtr->Scene, this, nullptr, nullptr, firstTile, 0);

		// Initial Culling Collection
		WindowPtr->Scene->DistantEntities.clear();
		TileGraph->GetDistantEntities(&WindowPtr->Scene->DistantEntities, 1);
	}

	WorldGenerator::~WorldGenerator()
	{
		delete TileGraph;

		if (OriginalTheme != nullptr)
			delete OriginalTheme;
		if (NextTheme != nullptr)
			delete NextTheme;
		delete CurrentTheme;		
	}

	void WorldGenerator::Update()
	{
		TimeElapsedSinceUpdate += Time::Delta;
		if (TimeElapsedSinceUpdate > UpdateTickDuration)
		{
			while (TimeElapsedSinceUpdate > UpdateTickDuration)
				TimeElapsedSinceUpdate -= UpdateTickDuration;

			// Update Tick
			auto character = WindowPtr->Input->GetCharacterEntity();
			if (character != nullptr)
			{
				auto c = TileGraph->FindClosest(character->Transform.Position);
				if (c == nullptr)
				{
					Debug::Error("Character not in a tile's volume.");
				}
				if (c != TileGraph)
				{
					// Update World Origin to new depth 0 tile origin


					c->UpdateDistance(0, nullptr);
					TileGraph = c;

					// Update Culled Entity Collection
					WindowPtr->Scene->DistantEntities.clear();
					TileGraph->GetDistantEntities(&WindowPtr->Scene->DistantEntities, 1);
				}
			}
		}

		if (ThemeTransitionDuration > -0.5f)
		{
			ThemeTransitionTimeElapsed += Time::Delta;
			if (ThemeTransitionTimeElapsed < ThemeTransitionDuration)
			{
				// Interpolate Current Theme
				CurrentTheme->AmbientLight = glm::mix(OriginalTheme->AmbientLight, NextTheme->AmbientLight, ThemeTransitionTimeElapsed / ThemeTransitionDuration);
				CurrentTheme->LightColor = glm::mix(OriginalTheme->LightColor, NextTheme->LightColor, ThemeTransitionTimeElapsed / ThemeTransitionDuration);
			}
			else
			{
				// Finish Transition
				CurrentTheme->AmbientLight = NextTheme->AmbientLight;
				CurrentTheme->LightColor = NextTheme->LightColor;
				ThemeTransitionDuration = -1.0f;
			}
		}
	}
	
	int WorldGenerator::GetMaxTileDepth()
	{
		return TileDepth;
	}

	std::string WorldGenerator::PickNextTile(std::string transitionType)
	{
		if (TilesByTransition.count(transitionType) == 0)
		{
			Debug::Log("No tiles found by transition type: " + transitionType);
			return "";
		}

		int weightTotal = 0;
		for (auto t : TilesByTransition[transitionType])
		{
			weightTotal += TileWeights[t];
		}

		int v = rand() % weightTotal;
		std::string tile;
		for (auto t : TilesByTransition[transitionType])
		{
			v -= TileWeights[t];
			if (v < 0)
			{
				tile = t;
				break;
			}
		}

		Debug::Log("Next Tile: " + tile);
		
		// Update Tile Weights
		for (auto t : TilesByTransition[transitionType])
		{
			if (t == tile)
			{
				TileWeights[t] = OriginalTileWeights[t];
			}
			else
			{
				TileWeights[t] += TileWeightAdjustments[t];
			}
		}

		return tile;
	}

	void WorldGenerator::BuildFeature(Tile* owner, Node* spawningNode)
	{
		if (FeaturesByType.count(spawningNode->Tag) == 0)
		{
			Debug::Log("No features found of type: " + spawningNode->Tag);
			return;
		}

		int weightTotal = 0;
		for (auto t : FeaturesByType[spawningNode->Tag])
		{
			weightTotal += FeatureWeights[t];
		}

		int v = rand() % weightTotal;
		Feature* feature = nullptr;
		for (auto t : FeaturesByType[spawningNode->Tag])
		{
			v -= FeatureWeights[t];
			if (v < 0)
			{
				feature = t;
				break;
			}
		}

		// Update Feature Weights
		for (auto t : FeaturesByType[spawningNode->Tag])
		{
			if (t == feature)
			{
				FeatureWeights[t] = OriginalFeatureWeights[t];
			}
			else
			{
				FeatureWeights[t] += FeatureWeightAdjustments[t];
			}
		}

		feature->BuildContents(WindowPtr->Scene, owner, spawningNode);
	}
	
	void WorldGenerator::ThemeTransition(Theme* nextTheme, float duration)
	{
		if (OriginalTheme != nullptr)
			delete OriginalTheme;
		if (NextTheme != nullptr)
			delete NextTheme;

		OriginalTheme = new Theme(*CurrentTheme);
		NextTheme = nextTheme;
		ThemeTransitionTimeElapsed = 0.0f;
		ThemeTransitionDuration = duration;
	}	



	void WorldGenerator::LoadTileDatabase()
	{
		std::string dbFile = "Resources/Tiles.bin";

		std::ifstream file(dbFile);
		if (!file)
		{
			Debug::Error("Invalid tile db.");
		}

		auto size = TilesByTransition.size();
		auto size2 = TilesByTransition.size();
		auto ss = dbFile.size();

		file.read((char*)&size, sizeof(size));
		for (unsigned int i = 0; i < size; i++)
		{
			std::string TransitonType;
			file.read((char*)&ss, sizeof(ss));
			TransitonType.resize(ss);
			file.read(&TransitonType[0], ss);

			file.read((char*)&size2, sizeof(size2));
			for (unsigned int j = 0; j < size2; j++)
			{
				std::string TileName;
				file.read((char*)&ss, sizeof(ss));
				TileName.resize(ss);
				file.read(&TileName[0], ss);

				TilesByTransition[TransitonType].push_back(TileName);
			}			
		}

		file.read((char*)&size, sizeof(size));
		for (unsigned int i = 0; i < size; i++)
		{
			std::string Name;
			file.read((char*)&ss, sizeof(ss));
			file.read((char*)&Name[0], ss);

			int weight, adjustment;
			file.read((char*)&weight, sizeof(weight));
			file.read((char*)&adjustment, sizeof(adjustment));

			OriginalTileWeights[Name] = TileWeights[Name] = weight;
			TileWeightAdjustments[Name] = adjustment;
		}

		file.close();
	}

	void WorldGenerator::BuildFeatureDatabase()
	{
		if (FeatureWeights.size() > 0)
		{
			FeaturesByType.clear();
			OriginalFeatureWeights.clear();
			FeatureWeightAdjustments.clear();

			for (auto f : FeatureWeights)
			{
				delete f.first;
			}
			FeatureWeights.clear();
		}

		ReadFeatureDirectory("Resources/Features/");
	}

	void WorldGenerator::ReadFeatureDirectory(std::string path)
	{
		// Open ImportDialog Window
		tinydir_dir dir;
		auto result = tinydir_open(&dir, path.c_str());
		if (result == 0)
		{
			while (dir.has_next)
			{
				tinydir_file file;
				tinydir_readfile(&dir, &file);

				if (file.is_dir && file.name[0] != '.')
				{
					// Add dir item
					ReadFeatureDirectory(file.path);
				}
				else if (!file.is_dir)
				{
					// Add item
					auto s = std::string(&file.path[20]);	// Ignore tile folder path: "Resources/Features/"
					
					Debug::Log("\t" + s);

					Feature* feature = new Feature(s);
					feature->Load();

					FeaturesByType[feature->GetTag()].push_back(feature);

					OriginalFeatureWeights[feature] = FeatureWeights[feature] = feature->GetSpawnWeight();
					FeatureWeightAdjustments[feature] = feature->GetSpawnWeightAdjustment();
				}

				tinydir_next(&dir);
			}

			tinydir_close(&dir);
		}
	}

	void WorldGenerator::BuildTileDatabase()
	{
		TilesByTransition.clear();
		TileWeights.clear();
		TileWeightAdjustments.clear();
		OriginalTileWeights.clear();

		ReadTileDirectory("Resources/Tiles/");

		/*
		std::ofstream file(dbFile);
		if (!file)
		{
			Debug::Error("Unable to save tile db.");
		}
		
		auto size = TilesByTransition.size();
		file.write((char*)&size, sizeof(size));					// # of Transition Types
		for (auto v : TilesByTransition)
		{
			size = v.first.size();
			file.write((char*)&size, sizeof(size));
			file.write((char*)&v.first[0], size);				// Name of Transition Type

			size = v.second.size();
			file.write((char*)&size, sizeof(size));				// # of tile that contain Transition
			for (auto t : v.second)
			{
				size = t.size();
				file.write((char*)&size, sizeof(size));
				file.write((char*)&t[0], size);					// Name of tile
			}
		}

		size = OriginalTileWeights.size();
		file.write((char*)&size, sizeof(size));					// Tile Weight Count
		for (auto w : OriginalTileWeights)
		{
			size = w.first.size();
			file.write((char*)&size, sizeof(size));
			file.write((char*)&w.first[0], size);				// Name of tile
			
			file.write((char*)&w.second, sizeof(w.second));		// Starting Weight
			file.write((char*)&TileWeightAdjustments[w.first], sizeof(TileWeightAdjustments[w.first]));
		}		

		file.close();
		*/
	}

	void WorldGenerator::ReadTileDirectory(std::string path)
	{
		// Open ImportDialog Window
		tinydir_dir dir;
		auto result = tinydir_open(&dir, path.c_str());
		if (result == 0)
		{
			while (dir.has_next)
			{
				tinydir_file file;
				tinydir_readfile(&dir, &file);

				if (file.is_dir && file.name[0] != '.')
				{
					// Add dir item
					ReadTileDirectory(file.path);
				}
				else if (!file.is_dir)
				{
					// Add item
					auto s = std::string(&file.path[17]);	// Ignore tile folder path: "Resources/Tiles/"
					
					Debug::Log("\t" + s);
					// Read transitions from tile and add them to the collections
					std::ifstream fs(file.path);

					int weight, adjustment;
					fs.read((char*)&weight, sizeof(weight));
					fs.read((char*)&adjustment, sizeof(adjustment));

					OriginalTileWeights[s] = TileWeights[s] = weight;
					TileWeightAdjustments[s] = adjustment;

					fs.seekg(sizeof(bool) + sizeof(glm::vec3) * 4 + sizeof(glm::quat), std::ios::cur);
					
					// Build Nodes
					auto size = TileWeights.size();
					auto ss = s.size();

					fs.read((char*)&size, sizeof(size));
					for (unsigned int i = 0; i < size; i++)
					{
						Node n;
						fs.read((char*)&ss, sizeof(ss));
						n.Tag.resize(ss);
						fs.read(&n.Tag[0], ss);

						fs.read((char*)&n.Transform.Position, sizeof(n.Transform.Position));
						fs.read((char*)&n.Transform.Rotation, sizeof(n.Transform.Rotation));
						fs.read((char*)&n.Transform.Scale, sizeof(n.Transform.Scale));

						TilesByTransition[n.Tag].push_back(s);
					}

					fs.close();
				}

				tinydir_next(&dir);
			}

			tinydir_close(&dir);
		}
	}
	
	void WorldGenerator::SetSpawnWeights(std::string tile, int currentWeight, int weight, int adjustment)
	{
		TileWeights[tile] = currentWeight;
		OriginalTileWeights[tile] = weight;
		TileWeightAdjustments[tile] = adjustment;
	}




	Theme::Theme() {}

	Theme::~Theme() {}

}