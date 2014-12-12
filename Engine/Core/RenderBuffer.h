#pragma once
#include "Core.h"
#include "Texture.h"

namespace Core
{

	enum class RenderBufferType { GBuffer, CBuffer, LightProbe, ShadowCubeMap, ShadowMap };

	class RenderBuffer
	{
	protected:
		RenderBufferType Type;
		GLuint FBO;
		std::vector<Texture*> ColorTextures;
		Texture* DepthTexture;
		glm::vec4 ClearColor;

	public:
		RenderBuffer(glm::vec4 clearColor, int colorBufferCount, RenderBufferType type);
		virtual ~RenderBuffer();

		virtual void MakeCurrent();
		virtual void Rebuild();
		virtual void Clear();
		virtual void SetCubeMapDepthTexture(Texture* out);
		virtual void SetCubeMapTexture(int i, Texture* out);

		GLuint GetOutputTexture(int i);
		GLuint GetDepthTexture();
	};

}
