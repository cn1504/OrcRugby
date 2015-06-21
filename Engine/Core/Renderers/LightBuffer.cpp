#include "LightBuffer.h"
using namespace Core::Renderers;
using namespace Core::Assets;

LightBuffer::LightBuffer(LightBuffer && o)
	: OffScreenBuffer(std::move(o))
{
	Luminance = std::move(o.Luminance);
}

LightBuffer& LightBuffer::operator=(LightBuffer && o)
{
	Luminance = std::move(o.Luminance);
	OffScreenBuffer::operator=(std::move(o));
	return *this;
}

LightBuffer::LightBuffer(const glm::ivec2& size)
	: OffScreenBuffer(size, std::make_unique<ClearToBlack>())
{
	Luminance = std::make_unique<Texture>();
	Build();
}

LightBuffer::~LightBuffer() {}

void LightBuffer::Scale(const glm::ivec2& delta)
{
	Size += delta;
	Build();
}

void LightBuffer::Build()
{
	if (Size.x == 0 || Size.y == 0)
		return;

	Luminance->CreateFPTexture(Size.x, Size.y);

	GLenum buffers[2];
	buffers[0] = GL_COLOR_ATTACHMENT0;

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Luminance->GetID(), 0);
	glDrawBuffers(1, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Debug->Error("Light RenderBuffer incomplete.");
	}

	Debug->GLError("ERROR: GLError on Light RenderBuffer build.");
}