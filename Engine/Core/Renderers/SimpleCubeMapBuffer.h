#pragma once
#include <Core.h>
#include "OffScreenBuffer.h"
#include <Assets/Texture.h>

namespace Core
{
	namespace Renderers
	{
		class SimpleCubeMapBuffer : public OffScreenBuffer
		{
		protected:
			void Build();

		public:
			std::unique_ptr<Core::Assets::Texture> Value;

			SimpleCubeMapBuffer() = delete;
			SimpleCubeMapBuffer(const SimpleCubeMapBuffer&) = delete;
			SimpleCubeMapBuffer& operator=(const SimpleCubeMapBuffer&) = delete;
			SimpleCubeMapBuffer(SimpleCubeMapBuffer &&);
			SimpleCubeMapBuffer& operator=(SimpleCubeMapBuffer &&);

			SimpleCubeMapBuffer(const glm::ivec2& size);
			virtual ~SimpleCubeMapBuffer();
			
			virtual void Scale(const glm::ivec2& delta);
		};
	}
}