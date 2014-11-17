#pragma once
#include "Core.h"
#include "Entity.h"

namespace Core
{

	class Component
	{
	public:
		Entity* Entity;

		virtual ~Component() {}
		virtual void Load() = 0;
		virtual void Update() = 0;
	};

}