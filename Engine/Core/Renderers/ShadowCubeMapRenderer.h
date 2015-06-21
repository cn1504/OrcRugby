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
			std::unique_ptr<Shader> SkeletalMesh;
			std::unique_ptr<Shader> PointCloud;	

			std::unique_ptr<Shader> StaticMesh;

		public:
			ShadowCubeMapRenderer();
			~ShadowCubeMapRenderer();

			void DrawScene();
			
			virtual void DrawMesh(const VertexArray& vao,
				const Core::Assets::Material& material, const glm::mat4& transform);

			virtual void DrawSea(const VertexArray& vao,
				const glm::mat4& transform);
		};
	}
}