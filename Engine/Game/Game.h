#pragma once

#include <Core.h>
#pragma comment(lib, "Core.lib")

#define GAME_VERSION "0.1.25"

namespace Game
{
	namespace Components
	{
		class SaveDB;
		class PrefsDB;
		class Log;
	}

	namespace States
	{
		class GameStateController;
	}

	extern std::shared_ptr<Game::Components::SaveDB> Save;
	extern std::shared_ptr<Game::Components::PrefsDB> Prefs;
	extern std::shared_ptr<Game::Components::Log> Log;
}