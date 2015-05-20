#pragma once

#include <Game.h>
#include "GameStateController.h"
#include <Components/AudioSource.h>

namespace Game
{
	namespace States
	{
		class MainMenuState : public GameState
		{
		private:
			std::shared_ptr<Core::Input::Action> EscAction;
			std::vector<std::shared_ptr<Core::Components::Gui::Item>> LoadedGuiComponents;
			std::shared_ptr<Core::Components::AudioSource> BGM;
			
			void Load();
			void Cleanup();

		public:
			MainMenuState(GameStateController* GSC);
			~MainMenuState();

			virtual void ContinueGame();
			virtual void NewGame();
			virtual void Close();
			virtual void Reload();
		};
	}
}