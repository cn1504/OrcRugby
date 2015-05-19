#pragma once

#include <Game.h>
#include <MainWindow.h>
#include <Input/Action.h>

namespace Game
{
	namespace Components
	{
		class GameState
		{
		protected:
			GameStateController* GSC;

		public:
			GameState(GameStateController* GSC) : GSC(GSC) {}
			virtual ~GameState() {}

			virtual void MainMenu() {}
			virtual void ContinueGame() {}
			virtual void NewGame() {}
			virtual void Close() {}
		};
		class InitialState;
		class MainMenuState;
		class InGameState;

		class GameStateController
		{
			friend class GameState;
			friend class InitialState;
			friend class MainMenuState;
			friend class InGameState;

		private:
			const glm::vec2 GUI_SIZE = glm::vec2(3840.0, 2160.0);

			Game::MainWindow* Window;
			std::unique_ptr<GameState> State;
			std::vector<std::shared_ptr<Core::Components::Gui::Item>> LoadedGuiComponents;
			std::vector<std::shared_ptr<Core::Space::TransformIF>> LoadedEntities;
			std::vector<std::shared_ptr<Core::Input::Action>> ActiveInputBindings;
			std::shared_ptr<Core::Input::Action> EscAction;

			std::vector<std::shared_ptr<Core::Components::Gui::Item>> GameMenuComponents;

			// Actions done by GameStates
			void LoadMainMenu();
			void LoadExistingGame();
			void LoadNewGame();

			void CloseMainMenu();
			void CloseGame();

		public:
			GameStateController(Game::MainWindow* wh);
			~GameStateController();
			
			// GameState Transitions
			void MainMenu() { State->MainMenu(); }
			void ContinueGame() { State->ContinueGame(); }
			void NewGame() { State->NewGame(); }
			void Close() { State->Close(); }

			void OpenGameMenu();
			void OpenOptionsMenu() {}
			void OpenInventory() {}
			void OpenEvent() {}

			void CloseGameMenu();
			void CloseOptionsMenu() {}
			void CloseInventory() {}
			void CloseEvent() {}
		};


		class InitialState : public GameState
		{
		public:
			InitialState(GameStateController* GSC) : GameState(GSC) {}

			virtual void MainMenu()	{ GSC->LoadMainMenu(); }
		};

		class MainMenuState : public GameState
		{
		public:
			MainMenuState(GameStateController* GSC) : GameState(GSC) {}

			virtual void ContinueGame()	{ GSC->CloseMainMenu(); GSC->LoadExistingGame(); }
			virtual void NewGame()	{ GSC->CloseMainMenu(); GSC->LoadNewGame(); }
			virtual void Close() { GSC->CloseMainMenu(); GSC->Window->Close(); }
		};

		class InGameState : public GameState
		{
		public:
			InGameState(GameStateController* GSC) : GameState(GSC) {}

			virtual void Close() { GSC->CloseGame(); GSC->LoadMainMenu(); }
		};


		class GSCAction : public Core::Input::Action
		{
		protected:
			GameStateController* GSC;
		public:
			GSCAction(GameStateController* GSC) : GSC(GSC) {}
		};

		// Main Menu Actions
		class CloseAction : public GSCAction
		{
		public:
			CloseAction(GameStateController* GSC) : GSCAction(GSC) {}
			virtual void Perform() { GSC->Close(); }
		};
		class NewGameAction : public GSCAction
		{
		public:
			NewGameAction(GameStateController* GSC) : GSCAction(GSC) {}
			virtual void Perform() { GSC->NewGame(); }
		};
		class ContinueGameAction : public GSCAction
		{
		public:
			ContinueGameAction(GameStateController* GSC) : GSCAction(GSC) {}
			virtual void Perform() { GSC->ContinueGame(); }
		};

		// InGame UI Actions
		class OpenGameMenuAction : public GSCAction
		{
		public:
			OpenGameMenuAction(GameStateController* GSC) : GSCAction(GSC) {}
			virtual void Perform() { GSC->OpenGameMenu(); }
		};
		class CloseGameMenuAction : public GSCAction
		{
		public:
			CloseGameMenuAction(GameStateController* GSC) : GSCAction(GSC) {}
			virtual void Perform() { GSC->CloseGameMenu(); }
		}; 
	}
}