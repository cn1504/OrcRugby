#pragma once

#include <Core.h>

namespace Core
{
	namespace Renderers
	{
		class VertexArray
		{
		protected:
			GLuint Id;

			GLuint ElementBuffer;
			GLsizei ElementBufferSize;

			std::unordered_map<GLuint, GLuint> Buffers;
			GLsizei BufferSize;

		public:
			VertexArray();
			virtual ~VertexArray();

			void SetBuffer(GLuint location, GLint vertexSize, GLenum type, GLboolean normalized, size_t bufferSize, const void* data);
			void SetElementBuffer(size_t bufferSize, const void* data);
			void RemoveBuffer(GLuint location);
			void RemoveElementBuffer();
			void Bind() const;
			void Unbind() const;
			GLsizei GetSize() const;
		};
	}
}