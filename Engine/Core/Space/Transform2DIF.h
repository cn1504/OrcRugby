#pragma once

#include "Translatable.h"
#include "Rotatable.h"
#include "Scalable.h"

namespace Core
{
	namespace Space
	{
		class Transform2DIF : public Translatable2D, public Rotatable2D, public Scalable2D,
			public std::enable_shared_from_this<Transform2DIF>
		{			
		public:
			virtual ~Transform2DIF() {}

			virtual void Translate(const glm::vec2& delta) = 0;
			virtual void Rotate(float delta) = 0;
			virtual void Scale(const glm::vec2& delta) = 0;
			virtual const glm::vec2& GetPosition() = 0;
			virtual const float& GetRotation() = 0;
			virtual const glm::vec2& GetScale() = 0;

			virtual void UpdateMatrix() = 0;
			virtual const glm::mat3& GetMatrix() = 0;
			virtual const glm::mat3& GetUnscaledMatrix() = 0;
			virtual glm::vec2 Down() = 0;
			virtual glm::vec2 Right() = 0;

			virtual void AddChild(std::shared_ptr<Transform2DIF> child) = 0;
			virtual void RemoveChild(std::shared_ptr<Transform2DIF> child) = 0;
			virtual std::shared_ptr<Transform2DIF> GetChild(size_t i) = 0;
			virtual size_t GetChildCount() = 0;
			virtual std::shared_ptr<Transform2DIF> GetParent() = 0;
			virtual void SetParent(std::shared_ptr<Transform2DIF> parent) = 0;
		};
	}
}