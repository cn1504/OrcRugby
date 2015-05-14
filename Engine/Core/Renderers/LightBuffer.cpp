#include "LightBuffer.h"
using namespace Core::Renderers;
using namespace Core::Assets;

LightBuffer::LightBuffer(LightBuffer && o)
	: OffScreenBuffer(std::move(o))
{
	Diffuse = std::move(o.Diffuse);
	Specular = std::move(o.Specular);
}

LightBuffer& LightBuffer::operator=(LightBuffer && o)
{
	Diffuse = std::move(o.Diffuse);
	Specular = std::move(o.Specular);
	OffScreenBuffer::operator=(std::move(o));
	return *this;
}

LightBuffer::LightBuffer(const glm::ivec2& size)
	: OffScreenBuffer(size, std::make_unique<ClearToBlack>())
{
	Diffuse = std::make_unique<Texture>();
	Specular = std::make_unique<Texture>();
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

	Diffuse->CreateTexture(false, Size.x, Size.y);
	Specular->CreateTexture(false, Size.x, Size.y);

	GLenum buffers[2];
	buffers[0] = GL_COLOR_ATTACHMENT0;
	buffers[1] = GL_COLOR_ATTACHMENT1;

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Diffuse->GetID(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, Specular->GetID(), 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Debug->Error("Light RenderBuffer incomplete.");
	}

	Debug->GLError("ERROR: GLError on Light RenderBuffer build.");
}