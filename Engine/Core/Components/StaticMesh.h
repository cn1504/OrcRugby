#pragma once
#include <Core.h>
#include "Entity.h"
#include <Renderers/VertexBuffer.h>
#include <Renderers/GeometryRenderer.h>

namespace Core
{
	namespace Components
	{
		class StaticMesh : public Entity
		{
		private:
			std::string Name;
			std::string Material;

		public:
			StaticMesh(std::string name, std::string material);
			virtual ~StaticMesh();
			
			virtual void Draw(Core::Renderers::GeometryRenderer* renderer);
		};
	}
}