#pragma once
#include "Core.h"
#include "Component.h"

namespace Core
{
	namespace Components
	{

		class FreeBody
			: public Component
		{
		public:
			FreeBody(Core::Scene::DynamicsWorld* world, Core::Assets::Material* material);
			virtual ~FreeBody();

			virtual void Load();
			virtual void Update();

		protected:
			DynamicsWorld* world;
			Material* material;

		};

	}
}