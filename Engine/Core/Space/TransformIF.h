#pragma once

#include "Translatable.h"
#include "Rotatable.h"
#include "Scalable.h"

namespace Core
{
	namespace Space
	{
		class TransformIF : public Translatable3D, public Rotatable3D, public Scalable3D,
			public std::enable_shared_from_this<TransformIF>
		{			
		public:
			virtual ~TransformIF() {}

			virtual void Translate(const glm::vec3& delta) = 0;
			virtual void Rotate(const glm::quat& delta) = 0;
			virtual void Rotate(const glm::vec3& deltaEuler) = 0;
			virtual void Scale(const glm::vec3& delta) = 0;
			virtual const glm::vec3& GetPosition() = 0;
			virtual const glm::quat& GetRotation() = 0;
			virtual const glm::vec3& GetScale() = 0;

			virtual void UpdateMatrix() = 0;
			virtual const glm::mat4& GetMatrix() = 0;
			virtual glm::vec3 Forward() = 0;
			virtual glm::vec3 Right() = 0;
			virtual glm::vec3 Up() = 0;

			virtual void AddChild(std::shared_ptr<TransformIF> child) = 0;
			virtual void RemoveChild(std::shared_ptr<TransformIF> child) = 0;
			virtual std::shared_ptr<TransformIF> GetChild(size_t i) = 0;
			virtual size_t GetChildCount() = 0;
			virtual std::shared_ptr<TransformIF> GetParent() = 0;
			virtual void SetParent(std::shared_ptr<TransformIF> parent) = 0;
		};
	}
}