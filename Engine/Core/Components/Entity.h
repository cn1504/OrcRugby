#pragma once

#include <Space/Transform.h>

namespace Core
{
	namespace Components
	{
		class Entity : public Core::Space::TransformIF
		{
		protected:
			std::shared_ptr<Core::Space::TransformIF> Transform;

		public:
			Entity() : Transform(std::make_shared<Core::Space::Transform>()) {}
			Entity(std::shared_ptr<Core::Space::TransformIF> transform) : Transform(transform) {}
			virtual ~Entity() {}

			// Transform Decorator Methods
			virtual void Translate(const glm::vec3& delta) { Transform->Translate(delta); }
			virtual void Rotate(const glm::quat& delta) { Transform->Rotate(delta); }
			virtual void Rotate(const glm::vec3& deltaEuler) { Transform->Rotate(deltaEuler); }
			virtual void Scale(const glm::vec3& delta) { Transform->Scale(delta); }
			virtual const glm::vec3& GetPosition() { return Transform->GetPosition(); }
			virtual const glm::quat& GetRotation() { return Transform->GetRotation(); }
			virtual const glm::vec3& GetScale() { return Transform->GetScale(); }

			virtual void UpdateMatrix() { Transform->UpdateMatrix(); }
			virtual const glm::mat4& GetMatrix() { return Transform->GetMatrix(); }
			virtual glm::vec3 Forward() { return Transform->Forward(); }
			virtual glm::vec3 Right() { return Transform->Right(); }
			virtual glm::vec3 Up() { return Transform->Up(); }

			virtual void AddChild(std::shared_ptr<TransformIF> child) {	Transform->AddChild(child);	}
			virtual void RemoveChild(std::shared_ptr<TransformIF> child) { Transform->RemoveChild(child); }
			virtual std::shared_ptr<TransformIF> GetChild(size_t i) { return Transform->GetChild(i); }
			virtual size_t GetChildCount() { return Transform->GetChildCount(); }
			virtual std::shared_ptr<TransformIF> GetParent() { return Transform->GetParent(); }
			virtual void SetParent(std::shared_ptr<TransformIF> parent) { return Transform->SetParent(parent); }
			
			// Entity Subtype Object Behaviors - overridden in child classes where appropriate
			//    Overridden functions must include a call to the function being overridden
			virtual void Update() {
				UpdateMatrix();

				for (size_t i = 0; i < Transform->GetChildCount(); i++)
				{
					auto child = std::static_pointer_cast<Entity>(Transform->GetChild(i));
					if (child != nullptr)
					{
						child->Update();
					}
				}
			}

			virtual void Draw(Core::Renderers::MeshRenderer* renderer) {
				for (size_t i = 0; i < Transform->GetChildCount(); i++)
				{
					auto child = std::static_pointer_cast<Entity>(Transform->GetChild(i));
					if (child != nullptr)
					{
						child->Draw(renderer);
					}
				}
			}

			virtual void DrawLights(Core::Renderers::LightRenderer* renderer) {
				for (size_t i = 0; i < Transform->GetChildCount(); i++)
				{
					auto child = std::static_pointer_cast<Entity>(Transform->GetChild(i));
					if (child != nullptr)
					{
						child->DrawLights(renderer);
					}
				}
			}
			
		};
	}
}