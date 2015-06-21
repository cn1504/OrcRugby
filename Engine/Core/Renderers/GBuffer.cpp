#include "GBuffer.h"

using namespace Core::Renderers;
using namespace Core::Assets;

GBuffer::GBuffer(GBuffer && o)
	: OffScreenBuffer(std::move(o))
{
	Depth = std::move(o.Depth);
	Normal = std::move(o.Normal);
	BaseColor = std::move(o.BaseColor);
	MSR = std::move(o.MSR);
}

GBuffer& GBuffer::operator=(GBuffer && o)
{
	Depth = std::move(o.Depth);
	Normal = std::move(o.Normal);
	BaseColor = std::move(o.BaseColor);
	MSR = std::move(o.MSR);
	OffScreenBuffer::operator=(std::move(o));
	return *this;
}

GBuffer::GBuffer(const glm::ivec2& size)
	: OffScreenBuffer(size, std::make_unique<ClearDepthAndToBlack>())
{
	Depth = std::make_unique<Texture>();
	Normal = std::make_unique<Texture>();
	BaseColor = std::make_unique<Texture>();
	MSR = std::make_unique<Texture>();
	Build();
}

GBuffer::~GBuffer() {}

void GBuffer::Scale(const glm::ivec2& delta)
{
	Size += delta;
	Build();
}

void GBuffer::Build()
{
	if (Size.x == 0 || Size.y == 0)
		return;

	Depth->CreateTexture(true, Size.x, Size.y);
	Normal->CreateFPTexture(Size.x, Size.y);
	BaseColor->CreateTexture(false, Size.x, Size.y);
	MSR->CreateTexture(false, Size.x, Size.y);

	GLenum buffers[3];
	buffers[0] = GL_COLOR_ATTACHMENT0;
	buffers[1] = GL_COLOR_ATTACHMENT1;
	buffers[2] = GL_COLOR_ATTACHMENT2;

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Depth->GetID(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Normal->GetID(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, BaseColor->GetID(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, MSR->GetID(), 0);
	glDrawBuffers(3, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Debug->Error("GBuffer incomplete.");
	}

	Debug->GLError("ERROR: GLError on GBuffer build.");
}