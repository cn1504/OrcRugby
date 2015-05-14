#pragma once
#include <Core.h>
#include <Scene/Entity.h>
#include "Component.h"

namespace Core
{
	namespace Components
	{
		class Camera
			: public Component
		{
		protected:
			glm::mat4 projection;

		public:
			Camera();
			virtual ~Camera();
			virtual void Load();
			virtual void Update();

			void UpdateProjection();
			glm::mat4 GetViewMatrix();
			glm::mat4 GetProjectionMatrix();
		};
	}
}