#pragma once
#include <Core.h>
#include "OffScreenBuffer.h"
#include <Assets/Texture.h>

namespace Core
{
	namespace Renderers
	{
		class ShadowBuffer : public OffScreenBuffer
		{
		protected:
			void Build();

		public:
			std::unique_ptr<Core::Assets::Texture> Depth;

			ShadowBuffer() = delete;
			ShadowBuffer(const ShadowBuffer&) = delete;
			ShadowBuffer& operator=(const ShadowBuffer&) = delete;
			ShadowBuffer(ShadowBuffer &&);
			ShadowBuffer& operator=(ShadowBuffer &&);

			ShadowBuffer(const glm::ivec2& size);
			virtual ~ShadowBuffer();
			
			virtual void Scale(const glm::ivec2& delta);
		};
	}
}