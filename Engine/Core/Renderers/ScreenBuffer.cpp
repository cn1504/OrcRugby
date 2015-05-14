#include "ScreenBuffer.h"

using namespace Core::Renderers;


ScreenBuffer::ScreenBuffer(ScreenBuffer && o)
	: RenderBuffer(std::move(o))
{
}

ScreenBuffer& ScreenBuffer::operator=(ScreenBuffer && o)
{
	RenderBuffer::operator=(std::move(o));
	return *this;
}

ScreenBuffer::ScreenBuffer(const glm::ivec2& size) 
	: RenderBuffer(size, std::make_unique<ClearDepthAndToBlack>())
{
}

ScreenBuffer::~ScreenBuffer() {}

void ScreenBuffer::SetAsTarget() 
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Size.x, Size.y);
}

void ScreenBuffer::Scale(const glm::ivec2& delta) 
{
	Size += delta;
}