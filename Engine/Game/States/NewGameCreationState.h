#pragma once

#include <Game.h>
#include "GameStateController.h"

namespace Game
{
	namespace States
	{
		class NewGameCreationState : public GameState
		{
		public:
			NewGameCreationState(GameStateController* GSC);
			~NewGameCreationState();
			virtual void Close();
		};
	}
}