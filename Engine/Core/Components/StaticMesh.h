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
			glm::vec4 Color;
			glm::vec4 MSR;

		public:
			StaticMesh(std::string name, const glm::vec4& color, const glm::vec4& msr);
			virtual ~StaticMesh();
			
			virtual void Draw(Core::Renderers::GeometryRenderer* renderer);
		};
	}
}