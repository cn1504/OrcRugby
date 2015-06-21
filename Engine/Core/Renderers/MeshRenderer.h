#pragma once

#include <Core.h>

namespace Core
{
	namespace Renderers
	{
		class MeshRenderer
		{
		public:
			virtual ~MeshRenderer() {}
						
			virtual void DrawMesh(const VertexArray& vao,
				const Core::Assets::Material& material, const glm::mat4& transform) = 0;

			virtual void DrawSea(const VertexArray& vao,
				const glm::mat4& transform) = 0;
		};
	}
}