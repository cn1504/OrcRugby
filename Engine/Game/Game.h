#pragma once

#include <Core.h>
#pragma comment(lib, "Core.lib")

#define GAME_VERSION "0.1.20"

namespace Game
{
	namespace Components
	{
		class SaveDB;
		class PrefsDB;
	}

	namespace States
	{
		class GameStateController;
	}

	extern std::shared_ptr<Game::Components::SaveDB> Save;
	extern std::shared_ptr<Game::Components::PrefsDB> Prefs;
}