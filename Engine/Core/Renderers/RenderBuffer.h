#pragma once
#include <Core.h>
#include <Space/Scalable.h>
#include "BufferClearBehavior.h"

namespace Core
{
	namespace Renderers
	{
		class RenderBuffer : public Core::Space::Scalable2DInt
		{
		protected:
			glm::ivec2 Size;
			std::unique_ptr<BufferClearBehavior> ClearBehavior;

		public:
			RenderBuffer() = delete;
			RenderBuffer(const RenderBuffer&) = delete;
			RenderBuffer& operator=(const RenderBuffer&) = delete;
			RenderBuffer(RenderBuffer &&);
			RenderBuffer& operator=(RenderBuffer &&);

			RenderBuffer(const glm::ivec2& size, std::unique_ptr<BufferClearBehavior> clear);
			virtual ~RenderBuffer();

			glm::ivec2 GetSize() { return Size; }

			void Clear();
			virtual void SetAsTarget() = 0;
			virtual void Scale(const glm::ivec2& delta) = 0;
		};
	}
}
