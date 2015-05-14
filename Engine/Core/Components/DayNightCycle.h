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

			std::unique_ptr<Core::Renderers::ShadowBuffer> ShadowBuffer;
			std::unique_ptr<Core::Renderers::ShadowMapRenderer> Renderer;

			glm::vec4 calculateColorFromTemp(float k);

		public:
			DayNightCycle(float timeRatio);
			virtual ~DayNightCycle();

			virtual void Update();
			virtual void DrawLights(Core::Renderers::LightRenderer* renderer);

			float GetCurrentTime();
		};
	}
}