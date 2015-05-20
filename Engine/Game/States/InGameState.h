#pragma once

#include <Game.h>
#include "GameStateController.h"

namespace Game
{
	namespace States
	{
		class CloseGameAction;

		class InGameState : public GameState
		{
			friend class CloseGameAction;

		private:
			std::vector<std::shared_ptr<Core::Components::Gui::Item>> GameMenuComponents;
			std::vector<std::shared_ptr<Core::Space::TransformIF>> LoadedEntities;
			std::vector<std::shared_ptr<Core::Components::Gui::Item>> LoadedGuiComponents;

			void CloseGameMenu();
			void CloseGame();

			void LoadGUI();
			void ReloadGUI();
			void CloseGUI();

			void SaveGame();

		public:
			InGameState(GameStateController* GSC);
			~InGameState();

			virtual void Close();
			virtual void OpenGameMenu();
			virtual void OpenOptionsMenu();
			virtual void OpenInventory();
			virtual void OpenEvent(std::string Event);
			virtual void Reload();
		};


		class CloseGameAction : public Core::Input::Action
		{
		private:
			InGameState* state;
		public:
			CloseGameAction(InGameState* s) : state(s) {}
			virtual void Perform();
		};
	}
}