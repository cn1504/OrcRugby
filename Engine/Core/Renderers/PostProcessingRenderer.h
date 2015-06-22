#pragma once
#include <Core.h>
#include "Shader.h"
#include "ScreenQuad.h"
#include "RenderBuffer.h"
#include "SimpleBuffer.h"

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

			std::unique_ptr<SimpleBuffer> AABuffer;

			std::unique_ptr<ScreenQuad> SQuad; 
			
			bool FXAAEnabled;

		public:
			PostProcessingRenderer();
			~PostProcessingRenderer();

			void SetTextures(const Core::Assets::Texture& luminance);
			void Draw(RenderBuffer& screenbuffer, const glm::ivec2& bufferSize);

			void ToggleFXAA();
		};
	}
}