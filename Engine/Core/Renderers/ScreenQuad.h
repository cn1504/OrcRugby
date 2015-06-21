#pragma once
#include <Core.h>

namespace Core
{
	namespace Renderers
	{
		class ScreenQuad
		{
		public:
			ScreenQuad();
			~ScreenQuad();
			void Render(const glm::ivec2& windowSize);
		};
	}
}