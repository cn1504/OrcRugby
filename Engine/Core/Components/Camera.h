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
			glm::mat4 InverseView;
			glm::mat4 Projection;
			glm::mat4 InverseProjection;
			glm::mat4 ViewProjection;
			glm::mat4 InverseViewProjection;

		public:
			Camera();
			virtual ~Camera();
			virtual void Load();
			virtual void Update();

			void UpdateProjection(float fovy, float aspectRatio, float minDrawDistance, float maxDrawDistance);
			const glm::mat4& GetView() const { return View; }
			const glm::mat4& GetProjection() const { return Projection; }
			const glm::mat4& GetInverseView() const { return InverseView; }
			const glm::mat4& GetInverseProjection() const { return InverseProjection; }
			const glm::mat4& GetViewProjection()  const { return ViewProjection; }
			const glm::mat4& GetInverseViewProjection()  const { return InverseViewProjection; }

			void GetWSFrustumCorners(std::vector<glm::vec3>& corners);
			glm::vec3 GetGridLocation();
		};
	}
}