#include "VertexArray.h"
#include <Window/Window.h>

using namespace Core::Renderers;

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &Id);
	glGenBuffers(1, &ElementBuffer);

	ElementBufferSize = 0;
	BufferSize = 0;
}

VertexArray::~VertexArray()
{
	for (auto& buffer : Buffers)
	{
		glDeleteBuffers(1, &buffer.second);
	}
	glDeleteBuffers(1, &ElementBuffer);
	glDeleteVertexArrays(1, &Id);
}

void VertexArray::SetBuffer(GLuint location, GLint vertexSize, GLenum type, GLboolean normalized, size_t bufferSize, const void* data)
{
	Bind();

	GLuint vbuffer;
	glGenBuffers(1, &vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(location);
	glVertexAttribPointer(
		location,                         // attribute
		vertexSize,                       // size
		type,							  // type
		normalized,                       // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	Buffers[location] = vbuffer;
	BufferSize = bufferSize / (vertexSize * (
		type == GL_FLOAT ? sizeof(float) : 
		type == GL_DOUBLE ? sizeof(double) : 
		type == GL_INT ? sizeof(int) :
		type == GL_SHORT ? sizeof(short) :
		type == GL_UNSIGNED_SHORT ? sizeof(unsigned short) :
		type == GL_UNSIGNED_INT ? sizeof(unsigned int) :
		sizeof(bool)
		));
	Unbind();
}

void VertexArray::SetElementBuffer(size_t bufferSize, const void* data)
{
	Bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, data, GL_STATIC_DRAW);
	ElementBufferSize = bufferSize / sizeof(unsigned short);
	Unbind();
}
void VertexArray::RemoveBuffer(GLuint location)
{
	if (Buffers.count(location) > 0)
	{
		Bind();
		glDisableVertexAttribArray(location);
		glDeleteBuffers(1, &Buffers[location]);
		Buffers.erase(location);
		Unbind();
	}
}
void VertexArray::RemoveElementBuffer()
{
	if (ElementBufferSize != 0)
	{
		Bind();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &ElementBuffer);
		ElementBufferSize = 0;
		Unbind();
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(Id);
}
void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

GLsizei VertexArray::GetSize() const
{
	return (ElementBufferSize > 0) ? ElementBufferSize : BufferSize;
}

