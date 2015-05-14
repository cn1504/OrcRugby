#include "SimpleCubeMapBuffer.h"
using namespace Core::Renderers;
using namespace Core::Assets;

SimpleCubeMapBuffer::SimpleCubeMapBuffer(SimpleCubeMapBuffer && o)
	: OffScreenBuffer(std::move(o))
{
	Value = std::move(o.Value);
}

SimpleCubeMapBuffer& SimpleCubeMapBuffer::operator=(SimpleCubeMapBuffer && o)
{
	Value = std::move(o.Value);
	OffScreenBuffer::operator=(std::move(o));
	return *this;
}

SimpleCubeMapBuffer::SimpleCubeMapBuffer(const glm::ivec2& size)
	: OffScreenBuffer(size, std::make_unique<ClearToBlack>())
{
	Value = std::make_unique<Texture>();
	Build();
}

SimpleCubeMapBuffer::~SimpleCubeMapBuffer() {}

void SimpleCubeMapBuffer::Scale(const glm::ivec2& delta)
{
	Size += delta;
	Build();
}

void SimpleCubeMapBuffer::Build()
{
	Value->CreateCubeMap(false, Size.x, Size.y);
	
	GLenum buffers[1];
	buffers[0] = GL_COLOR_ATTACHMENT0;

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Value->GetID(), 0);
	glDrawBuffers(1, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Debug->Error("Simple RenderBuffer incomplete.");
	}

	Debug->GLError("ERROR: GLError on Simple RenderBuffer build.");
}