#pragma once
#include <Core.h>
#include "RenderBuffer.h"

namespace Core
{
	namespace Renderers
	{
		class ScreenBuffer : public RenderBuffer
		{
		public:
			ScreenBuffer() = delete;
			ScreenBuffer(const ScreenBuffer&) = delete;
			ScreenBuffer& operator=(const ScreenBuffer&) = delete;
			ScreenBuffer(ScreenBuffer &&);
			ScreenBuffer& operator=(ScreenBuffer &&);

			ScreenBuffer(const glm::ivec2& size);
			virtual ~ScreenBuffer();

			virtual void SetAsTarget();
			virtual void Scale(const glm::ivec2& delta);
		};
	}
}