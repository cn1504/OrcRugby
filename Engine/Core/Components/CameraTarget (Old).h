#pragma once
#include "Core.h"
#include "Component.h"

namespace Core
{
	namespace Components
	{
		class CameraTarget
			: public Component
		{
		protected:
			Core::Entity* Target;

			float TransitionTimeElapsed;
			float TransitionDuration;
			glm::vec3 OriginalPosition;

		public:
			CameraTarget(Core::Entity* target);
			virtual ~CameraTarget();

			virtual void Load();
			virtual void Update();

			void SetTarget(Core::Entity* target);
		};
	}
}