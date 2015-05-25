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
						
			virtual void DrawMesh(const VertexBuffer& indices, const VertexBuffer& vertices,
				const VertexBuffer& uvs, const VertexBuffer& normals,
				const glm::vec4& color, const glm::vec4& msr, const glm::mat4& transform) = 0;
			
			virtual void DrawSea(const VertexBuffer& indices, const VertexBuffer& vertices,
				const VertexBuffer& uvs, const VertexBuffer& normals,
				const glm::mat4& transform) = 0;
		};
	}
}