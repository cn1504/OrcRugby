#pragma once

#include <Game.h>

namespace Game
{
	namespace Components
	{
		namespace Characters
		{
			class CharacterAI
			{
			private:
				Character* C;

				float Elapsed;
				float ActionDuration;

			public:
				CharacterAI(Character* c);

				void Update();
				Character* PickTarget(std::string category, std::string type);
			};
		}
	}
}
