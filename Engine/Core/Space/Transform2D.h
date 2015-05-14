#pragma once

#include "Transform2DIF.h"

namespace Core
{
	namespace Space
	{
		class Transform2D : public Transform2DIF
		{
		protected:
			glm::vec2 Position;
			float Rotation;
			glm::vec2 Scaling;
			glm::mat3 Unscaled;
			glm::mat3 LocalSpace;
			
			std::weak_ptr<Transform2DIF> Parent;
			std::vector<std::shared_ptr<Transform2DIF>> Children;
			
		public:
			Transform2D();
			Transform2D(const Transform2D&) = delete;
			Transform2D& operator=(const Transform2D&) = delete;

			virtual ~Transform2D();

			virtual void Translate(const glm::vec2& delta);
			virtual void Rotate(float delta);
			virtual void Scale(const glm::vec2& delta);
			virtual const glm::vec2& GetPosition();
			virtual const float& GetRotation();
			virtual const glm::vec2& GetScale();

			virtual void UpdateMatrix();
			virtual const glm::mat3& GetMatrix();
			virtual const glm::mat3& GetUnscaledMatrix();
			virtual glm::vec2 Down();
			virtual glm::vec2 Right();

			virtual void AddChild(std::shared_ptr<Transform2DIF> child);
			virtual void RemoveChild(std::shared_ptr<Transform2DIF> child);
			virtual std::shared_ptr<Transform2DIF> GetChild(size_t i);
			virtual size_t GetChildCount();
			virtual std::shared_ptr<Transform2DIF> GetParent();
			virtual void SetParent(std::shared_ptr<Transform2DIF> parent);
		};
	}
}