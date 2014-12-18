#include "RenderBuffer.h"

namespace Core
{

	RenderBuffer::RenderBuffer(glm::vec4 clearColor, int colorBufferCount, RenderBufferType type)
	{
		glGenFramebuffers(1, &FBO);
		Type = type;
		ClearColor = clearColor;

		if (Type == RenderBufferType::GBuffer)
		{
			DepthTexture = new Texture;
			
			for (int i = 0; i < colorBufferCount; i++)
			{
				auto t = new Texture;
				ColorTextures.push_back(t);
			}
		}
		else if (Type == RenderBufferType::CBuffer)
		{
			DepthTexture = nullptr;

			for (int i = 0; i < colorBufferCount; i++)
			{
				auto t = new Texture;
				ColorTextures.push_back(t);
			}
		}
		else if (Type == RenderBufferType::LightProbe)
		{
			DepthTexture = new Texture;
			DepthTexture->CreateCubeMap(true, (int)(Settings::Video::LightmapResolution), (int)(Settings::Video::LightmapResolution));

			ColorTextures.push_back(new Texture);
			ColorTextures[0]->CreateCubeMap(false, (int)(Settings::Video::LightmapResolution), (int)(Settings::Video::LightmapResolution));

			Rebuild();
		}
		else if (Type == RenderBufferType::ShadowCubeMap)
		{
			DepthTexture = new Texture;
			DepthTexture->CreateCubeMap(true, (int)(Settings::Video::ShadowResolution), (int)(Settings::Video::ShadowResolution));

			ColorTextures.push_back(new Texture);
			ColorTextures[0]->CreateCubeMap(false, (int)(Settings::Video::ShadowResolution), (int)(Settings::Video::ShadowResolution));

			Rebuild();
		}
		else if (Type == RenderBufferType::ShadowMap)
		{
			DepthTexture = new Texture;
			DepthTexture->CreateTexture(true, (int)(Settings::Video::ShadowResolution), (int)(Settings::Video::ShadowResolution));

			ColorTextures.push_back(new Texture);
			ColorTextures[0]->CreateTexture(false, (int)(Settings::Video::ShadowResolution), (int)(Settings::Video::ShadowResolution));

			Rebuild();
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

		if (Type == RenderBufferType::GBuffer || Type == RenderBufferType::CBuffer)
			glViewport(0, 0, (int)(Settings::Window::Width), (int)(Settings::Window::Height));

		else if (Type == RenderBufferType::ShadowCubeMap || Type == RenderBufferType::ShadowMap)
			glViewport(0, 0, (int)(Settings::Video::ShadowResolution), (int)(Settings::Video::ShadowResolution));

		else if (Type == RenderBufferType::LightProbe)
			glViewport(0, 0, (int)(Settings::Video::LightmapResolution), (int)(Settings::Video::LightmapResolution));
	}


	void RenderBuffer::Clear()
	{
		glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);
		glClear((Type == RenderBufferType::CBuffer) ? GL_COLOR_BUFFER_BIT : (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
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


		if (Type == RenderBufferType::ShadowCubeMap || Type == RenderBufferType::LightProbe)
		{
			if (Settings::Misc::VerboseLogging)
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


		if (Type == RenderBufferType::ShadowMap)
		{
			DepthTexture->CreateTexture(true, (int)(Settings::Video::ShadowResolution), (int)(Settings::Video::ShadowResolution));

			for (int i = 0; i < ColorTextures.size(); i++)
				ColorTextures[i]->CreateTexture(false, (int)(Settings::Video::ShadowResolution), (int)(Settings::Video::ShadowResolution));
		}		

		else {
			if (Type == RenderBufferType::GBuffer)
				DepthTexture->CreateTexture(true, (int)(Settings::Window::Width), (int)(Settings::Window::Height));

			for (int i = 0; i < ColorTextures.size(); i++)
			{
				ColorTextures[i]->CreateTexture(false, (int)(Settings::Window::Width), (int)(Settings::Window::Height));
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		if (Type == RenderBufferType::GBuffer || Type == RenderBufferType::ShadowMap)
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
		if (!(Type == RenderBufferType::ShadowCubeMap || Type == RenderBufferType::LightProbe))
			return;
		
		ColorTextures[i] = out;
	}


	void RenderBuffer::SetCubeMapDepthTexture(Texture* out)
	{
		if (!(Type == RenderBufferType::ShadowCubeMap || Type == RenderBufferType::LightProbe))
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