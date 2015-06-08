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
			public:
				void Update();
				Character* PickTarget(std::string category, std::string type);
			};
		}
	}
}
