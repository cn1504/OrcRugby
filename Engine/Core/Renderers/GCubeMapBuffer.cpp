#include "GCubeMapBuffer.h"

using namespace Core::Renderers;
using namespace Core::Assets;

GCubeMapBuffer::GCubeMapBuffer(GCubeMapBuffer && o)
	: OffScreenBuffer(std::move(o))
{
	Depth = std::move(o.Depth);
	Normal = std::move(o.Normal);
	BaseColor = std::move(o.BaseColor);
	MSR = std::move(o.MSR);
}

GCubeMapBuffer& GCubeMapBuffer::operator=(GCubeMapBuffer && o)
{
	Depth = std::move(o.Depth);
	Normal = std::move(o.Normal);
	BaseColor = std::move(o.BaseColor);
	MSR = std::move(o.MSR);
	OffScreenBuffer::operator=(std::move(o));
	return *this;
}

GCubeMapBuffer::GCubeMapBuffer(const glm::ivec2& size)
	: OffScreenBuffer(size, std::make_unique<ClearToBlack>())
{
	Depth = std::make_unique<Texture>();
	Normal = std::make_unique<Texture>();
	BaseColor = std::make_unique<Texture>();
	MSR = std::make_unique<Texture>();
	Build();
}

GCubeMapBuffer::~GCubeMapBuffer() {}

void GCubeMapBuffer::Scale(const glm::ivec2& delta)
{
	Size += delta;
	Build();
}

void GCubeMapBuffer::Build()
{
	Depth->CreateCubeMap(true, Size.x, Size.y);
	Normal->CreateCubeMap(false, Size.x, Size.y);
	BaseColor->CreateCubeMap(false, Size.x, Size.y);
	MSR->CreateCubeMap(false, Size.x, Size.y);

	GLenum buffers[3];
	buffers[0] = GL_COLOR_ATTACHMENT0;
	buffers[1] = GL_COLOR_ATTACHMENT1;
	buffers[2] = GL_COLOR_ATTACHMENT2;

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Normal->GetID(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, BaseColor->GetID(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, MSR->GetID(), 0);
	glDrawBuffers(3, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Debug->Error("Simple RenderBuffer incomplete.");
	}

	Debug->GLError("ERROR: GLError on Simple RenderBuffer build.");
}