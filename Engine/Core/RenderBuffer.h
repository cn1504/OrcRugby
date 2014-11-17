#pragma once
#include "Core.h"
#include "Texture.h"

namespace Core
{

	class RenderBuffer
	{
	protected:
		GLuint FBO;
		std::vector<Texture*> ColorTextures;
		Texture* DepthTexture;

		glm::vec4 ClearColor;
		bool IsCubeMap;
		bool HasDepthTexture;

	public:
		RenderBuffer(glm::vec4 clearColor, int colorBufferCount, bool hasDepthTexture = true, bool isCubeMap = false);
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
