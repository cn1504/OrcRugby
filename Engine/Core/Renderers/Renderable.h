#pragma once
#include <Core.h>

namespace Core
{
	namespace Renderers
	{

		class Renderable
		{
		public:
			virtual ~Renderable() {}
			virtual void Draw(Shader* shader) = 0;
		};

	}
}