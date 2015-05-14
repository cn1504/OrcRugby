#pragma once

#include <Game.h>
#include <MainWindow.h>

namespace Game
{
	namespace Components
	{
		class GameState;
		class InitialState;

		class GameStateController
		{
			friend class InitialState;

		private:
			Game::MainWindow* Window;
			std::unique_ptr<GameState> State;

			// Actions done by GameStates
			void LoadMainMenu();
			void LoadExistingGame();
			void LoadNewGame();
			void LoadInventory();
			void LoadEvent();

		public:
			GameStateController(Game::MainWindow* wh);
			~GameStateController();
			
			// GameState Transitions
			void MainMenu() { State->MainMenu(); }
			void ContinueGame() { State->ContinueGame(); }
			void NewGame() { State->NewGame(); }
			void OpenGameMenu() { State->OpenGameMenu(); }
			void OpenInventory() { State->OpenInventory(); }
			void OpenEvent() { State->OpenEvent(); }


		};


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
			virtual void OpenGameMenu() {}
			virtual void OpenInventory() {}
			virtual void OpenEvent() {}
		};

		class InitialState : public GameState
		{
		public:
			InitialState(GameStateController* GSC) : GameState(GSC) {}

			virtual void MainMenu()	{ GSC->LoadMainMenu(); }
		};
	}
}