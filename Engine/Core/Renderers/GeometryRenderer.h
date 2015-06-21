#pragma once
#include <Core.h>
#include "Shader.h"
#include "VertexArray.h"
#include <Components/Camera.h>
#include "MeshRenderer.h"

namespace Core
{
	namespace Renderers
	{
		class GeometryRenderer : public MeshRenderer
		{
		private:
			std::unique_ptr<Shader> LightGeometry;
			std::unique_ptr<Shader> SkeletalMesh;
			std::unique_ptr<Shader> PointCloud;
			std::unique_ptr<Shader> StaticMesh;

			std::unique_ptr<Shader> Sea;

			std::shared_ptr<Core::Components::Camera> Camera;
			glm::mat4 VP;

		public:
			GeometryRenderer();
			~GeometryRenderer();

			void SetCamera(std::shared_ptr<Core::Components::Camera> camera);
			void DrawScene();

			virtual void DrawMesh(const VertexArray& vao,
				const Core::Assets::Material& material, const glm::mat4& transform);

			virtual void DrawSea(const VertexArray& vao,
				const glm::mat4& transform);
		};
	}
}