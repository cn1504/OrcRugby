#pragma once

#include "TransformIF.h"

namespace Core
{
	namespace Space
	{
		class Transform : public TransformIF
		{
		protected:
			glm::vec3 Position;
			glm::quat Rotation;
			glm::vec3 Scaling;
			glm::mat4 Matrix;

			std::weak_ptr<TransformIF> Parent;
			std::vector<std::shared_ptr<TransformIF>> Children;

		public:
			Transform();
			Transform(const Transform&) = delete;
			Transform& operator=(const Transform&) = delete;

			virtual ~Transform();

			virtual void Translate(const glm::vec3& delta);
			virtual void Rotate(const glm::quat& delta);
			virtual void Rotate(const glm::vec3& deltaEuler);
			virtual void Scale(const glm::vec3& delta);
			virtual const glm::vec3& GetPosition();
			virtual const glm::quat& GetRotation();
			virtual const glm::vec3& GetScale();

			virtual void UpdateMatrix();
			virtual const glm::mat4& GetMatrix();
			virtual glm::vec3 Forward();
			virtual glm::vec3 Right();
			virtual glm::vec3 Up();

			virtual void AddChild(std::shared_ptr<TransformIF> child);
			virtual void RemoveChild(std::shared_ptr<TransformIF> child);
			virtual std::shared_ptr<TransformIF> GetChild(size_t i);
			virtual size_t GetChildCount();
			virtual std::shared_ptr<TransformIF> GetParent();
			virtual void SetParent(std::shared_ptr<TransformIF> parent);
		};
	}
}