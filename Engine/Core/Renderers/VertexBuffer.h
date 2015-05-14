#pragma once

#include <Core.h>

namespace Core
{
	namespace Renderers
	{
		class VertexBuffer
		{
		protected:
			GLuint id;
			size_t Size;

		public:
			VertexBuffer(size_t size, const void* data);
			virtual ~VertexBuffer();

			GLuint GetID() const;
			size_t GetSize() const;
			void Update(size_t size, const void* data);
		};
	}
}