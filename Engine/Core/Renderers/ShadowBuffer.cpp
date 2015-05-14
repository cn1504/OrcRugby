#include "ShadowBuffer.h"
using namespace Core::Renderers;
using namespace Core::Assets;

ShadowBuffer::ShadowBuffer(ShadowBuffer && o)
	: OffScreenBuffer(std::move(o))
{
	Depth = std::move(o.Depth);
	DepthMap = std::move(o.DepthMap);
}

ShadowBuffer& ShadowBuffer::operator=(ShadowBuffer && o)
{
	Depth = std::move(o.Depth);
	DepthMap = std::move(o.DepthMap);
	OffScreenBuffer::operator=(std::move(o));
	return *this;
}

ShadowBuffer::ShadowBuffer(const glm::ivec2& size)
	: OffScreenBuffer(size, std::make_unique<ClearDepthAndToBlack>())
{
	Depth = std::make_unique<Texture>();
	DepthMap = std::make_unique<Texture>();
	Build();
}

ShadowBuffer::~ShadowBuffer() {}

void ShadowBuffer::Scale(const glm::ivec2& delta)
{
	Size += delta;
	Build();
}

void ShadowBuffer::Build()
{
	Depth->CreateTexture(true, Size.x, Size.y);
	DepthMap->CreateTexture(false, Size.x, Size.y);

	GLenum buffers[1];
	buffers[0] = GL_COLOR_ATTACHMENT0;

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Depth->GetID(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, DepthMap->GetID(), 0);
	glDrawBuffers(1, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Debug->Error("Simple RenderBuffer incomplete.");
	}

	Debug->GLError("ERROR: GLError on Simple RenderBuffer build.");
}