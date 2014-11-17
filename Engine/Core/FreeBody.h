#pragma once
#include "Core.h"
#include "Component.h"

namespace Core
{
	class DynamicsWorld;
	class Material;

	class FreeBody
		: public Component
	{
	public:
		FreeBody(DynamicsWorld* world, Material* material);
		virtual ~FreeBody();

		virtual void Load();
		virtual void Update();

	protected:
		DynamicsWorld* world;
		Material* material;

	};
}