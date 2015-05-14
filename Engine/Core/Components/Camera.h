#pragma once

#include "Entity.h"

namespace Core
{
	namespace Components
	{
		class Camera : public Entity
		{
		protected:
			glm::mat4 View;
			glm::mat4 Projection;

		public:
			Camera();
			virtual ~Camera();
			virtual void Load();
			virtual void Update();

			void UpdateProjection(float fovy, float aspectRatio, float minDrawDistance, float maxDrawDistance);
			const glm::mat4& GetViewMatrix();
			const glm::mat4& GetProjectionMatrix();
		};
	}
}