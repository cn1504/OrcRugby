#pragma once
#include <Core.h>
#include <Components/Entity.h>
#include "Hex.h"

namespace Editor
{
	namespace Components
	{
		class Grid : public Core::Components::Entity
		{
		public:
			Grid();
			virtual ~Grid();

			std::shared_ptr<Hex> FindClosestHex(const glm::vec3& position);
		};
	}
}