#pragma once
#include "Core.h"

namespace Core
{

	class ScreenQuad
	{
	private:
		GLuint VertexBuffer;

	public:
		ScreenQuad();
		~ScreenQuad();
		void Render();
	};

}