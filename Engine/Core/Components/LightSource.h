#pragma once

#include <Core.h>
#include "Entity.h"
#include <Renderers/ShadowCubeMapBuffer.h>
#include <Renderers/ShadowCubeMapRenderer.h>

namespace Core
{
	namespace Components
	{
		class LightSource : public Entity
		{
		private:
			std::string Light;

			std::unique_ptr<Core::Renderers::ShadowCubeMapBuffer> ShadowBuffer;
			
		public:
			static std::unique_ptr<Core::Renderers::ShadowCubeMapRenderer> Renderer;
			static int LightCount;

			LightSource(std::string light);
			virtual ~LightSource();

			virtual void Update();
			virtual void DrawLights(Core::Renderers::LightRenderer* renderer);
		};
	}
}