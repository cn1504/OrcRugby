#pragma once

#include <Game.h>
#include <Components/Characters/Character.h>

namespace Game
{
	namespace Tests
	{
		class CharacterSpawner : public Core::Components::Entity
		{
		public:
			CharacterSpawner();
			virtual ~CharacterSpawner();

			virtual void Update();
		};
	}
}