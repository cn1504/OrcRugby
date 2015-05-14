#include "VertexBuffer.h"
#include <Window/Window.h>

using namespace Core::Renderers;

VertexBuffer::VertexBuffer(size_t size, const void* data)
{
	Size = size;

	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &id);
}

GLuint VertexBuffer::GetID() const
{ 
	return id; 
}

size_t VertexBuffer::GetSize() const
{
	return Size;
}

void VertexBuffer::Update(size_t size, const void* data)
{
	Size = size;
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

