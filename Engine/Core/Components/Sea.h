#pragma once
#include <Core.h>
#include "Entity.h"
#include <Renderers/VertexBuffer.h>
#include <Renderers/MeshRenderer.h>

namespace Core
{
	namespace Components
	{
		class Sea : public Entity
		{
		private:
			std::string Name;

		public:
			Sea(std::string name);
			virtual ~Sea();

			virtual void Draw(Core::Renderers::MeshRenderer* renderer);
		};
	}
}