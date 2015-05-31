#pragma once

#include <Game.h>
#include <Components/Entity.h>

namespace Game
{
	namespace Tests
	{
		class GameLogSpammer : public Core::Components::Entity
		{
		private:
			std::vector<std::string> testLines;
			size_t line;

			float timeElapsed;
			float waitDuration;

		public:
			GameLogSpammer();
			virtual ~GameLogSpammer();

			virtual void Update();
		};
	}
}