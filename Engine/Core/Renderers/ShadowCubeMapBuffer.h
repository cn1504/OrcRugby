#pragma once
#include <Core.h>
#include "OffScreenBuffer.h"
#include <Assets/Texture.h>

namespace Core
{
	namespace Renderers
	{
		class ShadowCubeMapBuffer : public OffScreenBuffer
		{
		protected:
			void Build();

		public:
			std::unique_ptr<Core::Assets::Texture> Depth;
			std::unique_ptr<Core::Assets::Texture> DepthMap;

			ShadowCubeMapBuffer() = delete;
			ShadowCubeMapBuffer(const ShadowCubeMapBuffer&) = delete;
			ShadowCubeMapBuffer& operator=(const ShadowCubeMapBuffer&) = delete;
			ShadowCubeMapBuffer(ShadowCubeMapBuffer &&);
			ShadowCubeMapBuffer& operator=(ShadowCubeMapBuffer &&);

			ShadowCubeMapBuffer(const glm::ivec2& size);
			virtual ~ShadowCubeMapBuffer();
			
			virtual void Scale(const glm::ivec2& delta);
		};
	}
}