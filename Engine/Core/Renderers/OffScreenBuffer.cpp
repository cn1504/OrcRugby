#include "OffScreenBuffer.h"
using namespace Core::Renderers;

OffScreenBuffer::OffScreenBuffer(OffScreenBuffer && o)
	: RenderBuffer(std::move(o))
{
	FBO = o.FBO;
}

OffScreenBuffer& OffScreenBuffer::operator=(OffScreenBuffer && o)
{
	FBO = o.FBO;
	RenderBuffer::operator=(std::move(o));
	return *this;
}

OffScreenBuffer::OffScreenBuffer(const glm::ivec2& size, std::unique_ptr<BufferClearBehavior> clear)
	: RenderBuffer(size, std::move(clear))
{
	glGenFramebuffers(1, &FBO);
}

OffScreenBuffer::~OffScreenBuffer() 
{
	glDeleteFramebuffers(1, &FBO);
}

void OffScreenBuffer::SetAsTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, Size.x, Size.y);
}