#pragma once

#include <Core.h>
#include "Entity.h"
#include <Renderers/ShadowBuffer.h>
#include <Renderers/ShadowMapRenderer.h>

namespace Core
{
	namespace Components
	{
		class DayNightCycle : public Entity
		{
		private:
			float TimeRatio;
			float CurrentTime;
			glm::vec3 SunDirection;
			glm::vec4 SunLightColor;
			glm::vec3 MoonDirection;
			glm::vec4 MoonLightColor;
			glm::vec4 SkyColor;
			float SunLightIntensity;
			float MoonLightIntensity;

			std::shared_ptr<Camera> camera;
			std::unique_ptr<Core::Renderers::ShadowMapRenderer> Renderer;
			float SunMaxDepth;
			glm::mat4 SunLightProjection;
			glm::mat4 SunLightView;
			float MoonMaxDepth;
			glm::mat4 MoonLightProjection;
			glm::mat4 MoonLightView;
			void RenderSceneToShadowmap();

			glm::vec4 calculateColorFromTemp(float k);

		public:
			std::unique_ptr<Core::Renderers::ShadowBuffer> SunBuffer;
			std::unique_ptr<Core::Renderers::ShadowBuffer> MoonBuffer;

			DayNightCycle(float timeRatio);
			virtual ~DayNightCycle();

			void SetCamera(std::shared_ptr<Camera> cam) { camera = cam; }

			virtual void Update();
			virtual void DrawLights(Core::Renderers::LightRenderer* renderer);

			float GetCurrentTime();
		};
	}
}