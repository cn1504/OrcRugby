#pragma once

#include <Game.h>
#include "GameStateController.h"
#include <Time/Timer.h>
#include <Components/Grid.h>

namespace Game
{
	namespace States
	{
		class CloseGameAction;
		class GameSpeedState;
		class PausedState;
		class NormalSpeedState;
		class DoubleSpeedState;
		class QuadSpeedState;

		
		class InGameState : public GameState
		{
			friend class CloseGameAction;

			friend class GameSpeedState;
			friend class PausedState;
			friend class NormalSpeedState;
			friend class DoubleSpeedState;
			friend class QuadSpeedState;

		private:
			std::vector<std::shared_ptr<Core::Components::Gui::Item>> GameMenuComponents;
			std::vector<std::shared_ptr<Core::Space::TransformIF>> LoadedEntities;
			std::vector<std::shared_ptr<Core::Components::Gui::Item>> LoadedGuiComponents;

			std::unique_ptr<GameSpeedState> SpeedState;
			void SetSpeedState(std::unique_ptr<GameSpeedState> s);
			std::shared_ptr<Core::Components::Gui::Panel> SpeedPanel;
			
			// Keybind actions
			std::shared_ptr<Core::Input::Action> TogglePauseKeybind;
			std::shared_ptr<Core::Input::Action> ExpandGrid;

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

			virtual void TogglePause();
			virtual void IncreaseSpeed();
			virtual void DecreaseSpeed();
		};


		class InGameStateAction : public Core::Input::Action
		{
		protected:
			InGameState* state;
		public:
			InGameStateAction(InGameState* s) : state(s) {}
			virtual void Perform() = 0;
		};

		class CloseGameAction : public InGameStateAction
		{
		public:
			CloseGameAction(InGameState* s) : InGameStateAction(s) {}
			virtual void Perform();
		};

		
		class IncreaseSpeedAction : public InGameStateAction
		{
		public:
			IncreaseSpeedAction(InGameState* s) : InGameStateAction(s) {}
			virtual void Perform();
		};
		class DecreaseSpeedAction : public InGameStateAction
		{
		public:
			DecreaseSpeedAction(InGameState* s) : InGameStateAction(s) {}
			virtual void Perform();
		};
		class TogglePauseAction : public InGameStateAction
		{
		public:
			TogglePauseAction(InGameState* s) : InGameStateAction(s) {}
			virtual void Perform();
		};


		class GameSpeedState
		{
		protected:
			InGameState* GS;

		public:
			GameSpeedState(InGameState* GS) : GS(GS) {}
			
			virtual void Increase() {}
			virtual void Decrease() {}
			virtual void TogglePause();
			
			virtual std::string GetPanelImage() = 0;
			virtual float GetTimeMultiplier() = 0;
		};

		class PausedState : public GameSpeedState
		{
		private:
			std::unique_ptr<GameSpeedState> PreviousState;
		public:
			PausedState(InGameState* GS) : GameSpeedState(GS) { PreviousState = std::move(GS->SpeedState); }
			virtual void Increase();
			virtual void TogglePause();
			virtual std::string GetPanelImage();
			virtual float GetTimeMultiplier();
		};
		class NormalSpeedState : public GameSpeedState
		{
		public:
			NormalSpeedState(InGameState* GS) : GameSpeedState(GS) {}
			virtual void Increase();
			virtual void Decrease();
			virtual std::string GetPanelImage();
			virtual float GetTimeMultiplier();
		};
		class DoubleSpeedState : public GameSpeedState
		{
		public:
			DoubleSpeedState(InGameState* GS) : GameSpeedState(GS) {}
			virtual void Increase();
			virtual void Decrease();
			virtual std::string GetPanelImage();
			virtual float GetTimeMultiplier();
		};
		class QuadSpeedState : public GameSpeedState
		{
		public:
			QuadSpeedState(InGameState* GS) : GameSpeedState(GS) {}
			virtual void Decrease();

			virtual std::string GetPanelImage();
			virtual float GetTimeMultiplier();
		};


		class ExpandGridAction : public Core::Input::Action
		{
		protected:
			std::weak_ptr<Game::Components::Grid> Grid;
			std::weak_ptr<Core::Space::TransformIF> Target;

		public:
			ExpandGridAction(std::weak_ptr<Game::Components::Grid> grid, std::weak_ptr<Core::Space::TransformIF> target);
			virtual ~ExpandGridAction();
			virtual void Perform();
		};
	}
}