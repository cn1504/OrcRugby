#pragma once
#include <Core.h>
#include "Shader.h"
#include <Components/Camera.h>
#include "ScreenQuad.h"

namespace Core
{
	namespace Renderers
	{
		class LightRenderer
		{
		private:
			std::unique_ptr<Shader> Light;
			std::unique_ptr<Shader> LightWithShadow;
			std::unique_ptr<Shader> DirectionalLight;

			std::unique_ptr<ScreenQuad> SQuad;

			std::shared_ptr<Core::Components::Camera> Camera;
			glm::ivec2 CurrentBufferSize;
			glm::mat4 VP;
			glm::mat4 PInverse;
			glm::mat4 VInverse;

		public:
			LightRenderer();
			~LightRenderer();

			void SetCamera(std::shared_ptr<Core::Components::Camera> camera);
			void SetGeometryTextures(const Core::Assets::Texture& depth, const Core::Assets::Texture& normal, const Core::Assets::Texture& base, const Core::Assets::Texture& msr);
			void DrawScene(const glm::vec2& bufferSize);
			void DrawLight(const glm::mat4& transform, const Core::Assets::Light& light);
			void DrawLight(const Core::Assets::Texture& shadowMap, const glm::mat4& transform, const Core::Assets::Light& light);
			void DrawLight(const Core::Assets::Texture& shadowMap, const glm::vec3& direction, const glm::vec4& color, float intensity);
		};
	}
}