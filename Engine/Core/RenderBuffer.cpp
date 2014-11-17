#include "RenderBuffer.h"

namespace Core
{

	RenderBuffer::RenderBuffer(glm::vec4 clearColor, int colorBufferCount, bool hasDepthTexture, bool isCubeMap)
	{
		glGenFramebuffers(1, &FBO);
		ClearColor = clearColor;
		HasDepthTexture = hasDepthTexture;
		IsCubeMap = isCubeMap;
		DepthTexture = nullptr;

		if (IsCubeMap)
		{
			HasDepthTexture = true;
			DepthTexture = new Texture;
			DepthTexture->CreateCubeMap(true, (int)(Settings::Video::ShadowResolution), (int)(Settings::Video::ShadowResolution));

			ColorTextures.push_back(new Texture);
			ColorTextures[0]->CreateCubeMap(false, (int)(Settings::Video::ShadowResolution), (int)(Settings::Video::ShadowResolution));

			Rebuild();
		}
		else
		{
			if (HasDepthTexture)
			{
				DepthTexture = new Texture;
			}

			for (int i = 0; i < colorBufferCount; i++)
			{
				auto t = new Texture;
				ColorTextures.push_back(t);
			}
		}
	}


	RenderBuffer::~RenderBuffer()
	{	
		glDeleteFramebuffers(1, &FBO);	

		for (auto t : ColorTextures)
			delete t;
		if (DepthTexture != nullptr)
			delete DepthTexture;
	}


	void RenderBuffer::MakeCurrent()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		if (IsCubeMap)
			glViewport(0, 0, (int)(Settings::Video::ShadowResolution), (int)(Settings::Video::ShadowResolution));
		else
			glViewport(0, 0, (int)(Settings::Window::Width), (int)(Settings::Window::Height));
	}


	void RenderBuffer::Clear()
	{
		glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);
		glClear((HasDepthTexture) ? (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT) : GL_COLOR_BUFFER_BIT);
	}


	void RenderBuffer::Rebuild()
	{
		GLenum buffers[8];
		buffers[0] = GL_COLOR_ATTACHMENT0;
		buffers[1] = GL_COLOR_ATTACHMENT1;
		buffers[2] = GL_COLOR_ATTACHMENT2;
		buffers[3] = GL_COLOR_ATTACHMENT3;
		buffers[4] = GL_COLOR_ATTACHMENT4;
		buffers[5] = GL_COLOR_ATTACHMENT5;
		buffers[6] = GL_COLOR_ATTACHMENT6;
		buffers[7] = GL_COLOR_ATTACHMENT7;
		buffers[8] = GL_COLOR_ATTACHMENT8;


		if (IsCubeMap)
		{
			Debug::Log("Building Cubemap... Textures: " + std::to_string(ColorTextures.size()));
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);

			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, DepthTexture->GetID(), 0);
			for (int i = 0; i < ColorTextures.size(); i++)
				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, ColorTextures[i]->GetID(), 0);
			glDrawBuffers((GLsizei)ColorTextures.size(), buffers);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				Debug::Error("Cubemap RenderBuffer incomplete.");
			}

			return;
		}
		

		if (HasDepthTexture)
			DepthTexture->CreateTexture(true, (int)(Settings::Window::Width), (int)(Settings::Window::Height));
		for (int i = 0; i < ColorTextures.size(); i++)
			ColorTextures[i]->CreateTexture(false, (int)(Settings::Window::Width), (int)(Settings::Window::Height));

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		if (HasDepthTexture)
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, DepthTexture->GetID(), 0);

		for (int i = 0; i < ColorTextures.size(); i++)
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, ColorTextures[i]->GetID(), 0);
		glDrawBuffers((GLsizei)ColorTextures.size(), buffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Debug::Error("RenderBuffer incomplete.");
		}

		Debug::GLError("ERROR: GLError on RenderBuffer Rebuild.");
	}
	

	void RenderBuffer::SetCubeMapTexture(int i, Texture* out)
	{
		if (!IsCubeMap)
			return;
		
		ColorTextures[i] = out;
	}


	void RenderBuffer::SetCubeMapDepthTexture(Texture* out)
	{
		if (!IsCubeMap)
			return;

		DepthTexture = out;
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, DepthTexture->GetID(), 0);
	}
	

	GLuint RenderBuffer::GetOutputTexture(int i)
	{
		return ColorTextures[i]->GetID();
	}


	GLuint RenderBuffer::GetDepthTexture()
	{
		return (DepthTexture != nullptr) ? DepthTexture->GetID() : -1;
	}

}