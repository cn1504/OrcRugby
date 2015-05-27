#pragma once
#include <Core.h>
#include "Shader.h"
#include "MeshRenderer.h"

namespace Core
{
	namespace Renderers
	{
		class ShadowCubeMapRenderer : public MeshRenderer
		{
		private:
			std::unique_ptr<Shader> Mesh;
			std::unique_ptr<Shader> SkeletalMesh;
			std::unique_ptr<Shader> Sphere;
			std::unique_ptr<Shader> Cylinder;
			std::unique_ptr<Shader> PointCloud;	

			std::unique_ptr<Shader> ColoredMesh;
			std::unique_ptr<Shader> StaticMesh;

		public:
			ShadowCubeMapRenderer();
			~ShadowCubeMapRenderer();

			void DrawScene();
			
			virtual void DrawMesh(const VertexBuffer& indices, const VertexBuffer& vertices,
				const VertexBuffer& uvs, const VertexBuffer& normals,
				const Core::Assets::Material& material, const glm::mat4& transform);

			virtual void DrawSea(const VertexBuffer& indices, const VertexBuffer& vertices,
				const VertexBuffer& uvs, const VertexBuffer& normals,
				const glm::mat4& transform);
		};
	}
}