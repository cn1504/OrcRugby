#pragma once
#include <Core.h>
#include "Shader.h"
#include "ScreenQuad.h"

namespace Core
{
	namespace Renderers
	{
		class PostProcessingRenderer
		{
		private:
			std::unique_ptr<Shader> CombineBuffers;
			std::unique_ptr<Shader> Blur;
			std::unique_ptr<Shader> NoAA;
			std::unique_ptr<Shader> FXAA;
			
			std::unique_ptr<ScreenQuad> SQuad;


		public:
			PostProcessingRenderer();
			~PostProcessingRenderer();

			void SetTextures(const Core::Assets::Texture& diffuse, const Core::Assets::Texture& specular);
			void Draw(const glm::ivec2& bufferSize);
		};
	}
}