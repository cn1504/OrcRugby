#pragma once
#include <Core.h>
#include "RenderBuffer.h"

namespace Core
{
	namespace Renderers
	{
		class OffScreenBuffer : public RenderBuffer
		{
		protected:
			GLuint FBO;

		public:
			OffScreenBuffer() = delete;
			OffScreenBuffer(const OffScreenBuffer&) = delete;
			OffScreenBuffer& operator=(const OffScreenBuffer&) = delete;
			OffScreenBuffer(OffScreenBuffer &&);
			OffScreenBuffer& operator=(OffScreenBuffer &&);

			OffScreenBuffer(const glm::ivec2& size, std::unique_ptr<BufferClearBehavior> clear);
			virtual ~OffScreenBuffer();

			virtual void SetAsTarget();
			virtual void Scale(const glm::ivec2& delta) = 0;
		};
	}
}