#pragma once
#include <Core.h>
#include "OffScreenBuffer.h"
#include <Assets/Texture.h>

namespace Core
{
	namespace Renderers
	{
		class GCubeMapBuffer : public OffScreenBuffer
		{
		protected:
			void Build();

		public:
			std::unique_ptr<Core::Assets::Texture> Depth;
			std::unique_ptr<Core::Assets::Texture> Normal;
			std::unique_ptr<Core::Assets::Texture> BaseColor;
			std::unique_ptr<Core::Assets::Texture> MSR;

			GCubeMapBuffer() = delete;
			GCubeMapBuffer(const GCubeMapBuffer&) = delete;
			GCubeMapBuffer& operator=(const GCubeMapBuffer&) = delete;
			GCubeMapBuffer(GCubeMapBuffer &&);
			GCubeMapBuffer& operator=(GCubeMapBuffer &&);

			GCubeMapBuffer(const glm::ivec2& size);
			virtual ~GCubeMapBuffer();
			
			virtual void Scale(const glm::ivec2& delta);
		};
	}
}