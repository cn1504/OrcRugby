#pragma once

#include <Core.h>
#pragma comment(lib, "Core.lib")

namespace Game
{
	namespace Components
	{
		class SaveDB;
		class PrefsDB;

		class GameStateController;
	}

	extern std::shared_ptr<Game::Components::SaveDB> Save;
	extern std::shared_ptr<Game::Components::PrefsDB> Prefs;
}