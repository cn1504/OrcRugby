#include "RenderBuffer.h"
using namespace Core::Renderers;

RenderBuffer::RenderBuffer(RenderBuffer && o)
{
	Size = o.Size;
	ClearBehavior = std::move(o.ClearBehavior);
}

RenderBuffer& RenderBuffer::operator=(RenderBuffer && o)
{
	Size = o.Size;
	ClearBehavior = std::move(o.ClearBehavior);
	
	return *this;
}

RenderBuffer::RenderBuffer(const glm::ivec2& size, std::unique_ptr<BufferClearBehavior> clear)
	: Size(size), ClearBehavior(std::move(clear))
{
}

RenderBuffer::~RenderBuffer() {}

void RenderBuffer::Clear()
{
	ClearBehavior->Clear();
}
