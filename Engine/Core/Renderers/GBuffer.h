#pragma once
#include <Core.h>
#include "OffScreenBuffer.h"
#include <Assets/Texture.h>

namespace Core
{
	namespace Renderers
	{
		class GBuffer : public OffScreenBuffer
		{
		protected:
			void Build();

		public:
			std::unique_ptr<Core::Assets::Texture> Depth;
			std::unique_ptr<Core::Assets::Texture> Normal;
			std::unique_ptr<Core::Assets::Texture> BaseColor;
			std::unique_ptr<Core::Assets::Texture> MSR;

			GBuffer() = delete;
			GBuffer(const GBuffer&) = delete;
			GBuffer& operator=(const GBuffer&) = delete;
			GBuffer(GBuffer &&);
			GBuffer& operator=(GBuffer &&);

			GBuffer(const glm::ivec2& size);
			virtual ~GBuffer();
			
			virtual void Scale(const glm::ivec2& delta);
		};
	}
}