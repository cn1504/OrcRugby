#pragma once
#include "Core.h"
#include "Component.h"
#include "Shader.h"

namespace Core
{

	class Renderable
		: public Component
	{
	public:
		virtual ~Renderable() {}
		virtual void Load() = 0;
		virtual void Update() = 0;

		virtual void EnableBuffers(Shader* shader) = 0;
		virtual void Render(Shader* shader) = 0;
		virtual void DisableBuffers(Shader* shader) = 0;
	};

}