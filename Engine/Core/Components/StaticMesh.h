#pragma once
#include <Core.h>
#include "Entity.h"
#include <Renderers/VertexArray.h>
#include <Renderers/MeshRenderer.h>

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

			virtual void Draw(Core::Renderers::MeshRenderer* renderer);
		};
	}
}