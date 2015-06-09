#pragma once

#include <Core.h>
#pragma comment(lib, "Core.lib")

#define GAME_VERSION "0.1.27"

namespace Game
{
	namespace Components
	{
		class SaveDB;
		class PrefsDB;
		class Log;
		class Generator;

		namespace Characters
		{
			class Item;
			class Character;
			class Class;
			class StatBlock;
			class Inventory;
			class ItemStatBonus;
			class Power;
		}
	}

	namespace States
	{
		class GameStateController;
	}

	extern std::shared_ptr<Game::Components::SaveDB> Save;
	extern std::shared_ptr<Game::Components::PrefsDB> Prefs;
	extern std::shared_ptr<Game::Components::Log> Log;
	extern std::shared_ptr<Game::Components::Generator> Generator;

	extern std::unordered_map<std::string, std::vector<std::shared_ptr<Game::Components::Characters::Character>>> Factions;
}