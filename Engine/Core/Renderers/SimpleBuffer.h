#pragma once
#include <Core.h>
#include "OffScreenBuffer.h"
#include <Assets/Texture.h>

namespace Core
{
	namespace Renderers
	{
		class SimpleBuffer : public OffScreenBuffer
		{
		protected:
			void Build();

		public:
			std::unique_ptr<Core::Assets::Texture> Value;

			SimpleBuffer() = delete;
			SimpleBuffer(const SimpleBuffer&) = delete;
			SimpleBuffer& operator=(const SimpleBuffer&) = delete;
			SimpleBuffer(SimpleBuffer &&);
			SimpleBuffer& operator=(SimpleBuffer &&);

			SimpleBuffer(const glm::ivec2& size);
			virtual ~SimpleBuffer();
			
			virtual void Scale(const glm::ivec2& delta);
		};
	}
}