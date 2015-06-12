#pragma once
#include <Core.h>
#include "GeometryRenderer.h"
#include "Shader.h"

namespace Core
{
	namespace Renderers
	{
		class ShadowMapRenderer : public MeshRenderer
		{
		private:
			/*
			std::unique_ptr<Shader> Mesh;
			std::unique_ptr<Shader> SkeletalMesh;
			std::unique_ptr<Shader> Sphere;
			std::unique_ptr<Shader> Cylinder;
			std::unique_ptr<Shader> PointCloud;
			*/

			std::unique_ptr<Shader> StaticMesh;
			std::unique_ptr<Shader> Sea;

			glm::mat4 Projection;
			glm::mat4 View;
			float MaxDepth;

		public:
			ShadowMapRenderer();
			~ShadowMapRenderer();

			void DrawScene();
			
			virtual void DrawMesh(const VertexBuffer& indices, const VertexBuffer& vertices,
				const VertexBuffer& uvs, const VertexBuffer& normals,
				const Core::Assets::Material& material, const glm::mat4& transform);

			virtual void DrawSea(const VertexBuffer& indices, const VertexBuffer& vertices,
				const VertexBuffer& uvs, const VertexBuffer& normals,
				const glm::mat4& transform);

			void SetMatrices(const glm::mat4& projection, const glm::mat4& view, float maxDepth);
		};
	}
}