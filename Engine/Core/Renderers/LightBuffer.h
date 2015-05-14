#pragma once
#include <Core.h>
#include "OffScreenBuffer.h"
#include <Assets/Texture.h>

namespace Core
{
	namespace Renderers
	{
		class LightBuffer : public OffScreenBuffer
		{
		protected:
			void Build();

		public:
			std::unique_ptr<Core::Assets::Texture> Diffuse;
			std::unique_ptr<Core::Assets::Texture> Specular;

			LightBuffer() = delete;
			LightBuffer(const LightBuffer&) = delete;
			LightBuffer& operator=(const LightBuffer&) = delete;
			LightBuffer(LightBuffer &&);
			LightBuffer& operator=(LightBuffer &&);

			LightBuffer(const glm::ivec2& size);
			virtual ~LightBuffer();
			
			virtual void Scale(const glm::ivec2& delta);
		};
	}
}