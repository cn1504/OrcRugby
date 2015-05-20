#pragma once

#include <Game.h>
#include <MainWindow.h>
#include <Input/Action.h>

namespace Game
{
	namespace States
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
			virtual void Reload() {}

			virtual void OpenGameMenu() {}
			virtual void OpenOptionsMenu() {}
			virtual void OpenInventory() {}
			virtual void OpenEvent(std::string Event) {}
		};
		class InitialState;
		class MainMenuState;
		class NewGameCreationState;
		class InGameState;

		class GameStateController
		{
			friend class InitialState;
			friend class MainMenuState;
			friend class InGameState; 
			friend class NewGameCreationState;

		private:
			const glm::vec2 GUI_SIZE = glm::vec2(3840.0, 2160.0);

			Game::MainWindow* Window;
			std::unique_ptr<GameState> State;
			std::shared_ptr<Core::Input::Action> EscAction;
			
		public:
			GameStateController(Game::MainWindow* wh);
			~GameStateController();
			
			// GameState Transitions
			void MainMenu() { State->MainMenu(); }
			void ContinueGame() { State->ContinueGame(); }
			void NewGame() { State->NewGame(); }
			void Close() { State->Close(); }
			void Reload() { State->Reload(); }

			void OpenGameMenu() { State->OpenGameMenu(); }
			void OpenOptionsMenu() { State->OpenOptionsMenu(); }
			void OpenInventory() { State->OpenInventory(); }
			void OpenEvent(std::string Event) { State->OpenEvent(Event); }
		};


		class InitialState : public GameState
		{
		public:
			InitialState(GameStateController* GSC) : GameState(GSC) {}
			virtual void MainMenu();
		};
		

		class GSCAction : public Core::Input::Action
		{
		protected:
			GameStateController* GSC;
		public:
			GSCAction(GameStateController* GSC) : GSC(GSC) {}
		};
		class CloseAction : public GSCAction
		{
		public:
			CloseAction(GameStateController* GSC) : GSCAction(GSC) {}
			virtual void Perform() { GSC->Close(); }
		};

		// Main Menu Actions		
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
		class OpenInventoryAction : public GSCAction
		{
		public:
			OpenInventoryAction(GameStateController* GSC) : GSCAction(GSC) {}
			virtual void Perform() { GSC->OpenInventory(); }
		};
		class OpenEventAction : public GSCAction
		{
		private:
			std::string Event;
		public:
			OpenEventAction(GameStateController* GSC, std::string Event) : GSCAction(GSC), Event(Event) {}
			virtual void Perform() { GSC->OpenEvent(Event); }
		};
	}
}