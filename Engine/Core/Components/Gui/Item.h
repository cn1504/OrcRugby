#pragma once
#include <Core.h>
#include <Space/Transform2D.h>

namespace Core
{
	namespace Components
	{
		namespace Gui
		{
			// Item decorates Transform2D
			class Item : public Core::Space::Transform2DIF
			{
			protected:
				std::shared_ptr<Core::Space::Transform2DIF> Transform;

			public:
				Item() : Transform(std::make_shared<Core::Space::Transform2D>()) {}
				Item(std::shared_ptr<Core::Space::Transform2DIF> transform) : Transform(transform) {}
				virtual ~Item() {}
				
				virtual void Translate(const glm::vec2& delta) { Transform->Translate(delta); }
				virtual void Rotate(float delta) { Transform->Rotate(delta); }
				virtual void Scale(const glm::vec2& delta) { Transform->Scale(delta); }
				virtual const glm::vec2& GetPosition() { return Transform->GetPosition(); }
				virtual const float& GetRotation() { return Transform->GetRotation(); }
				virtual const glm::vec2& GetScale() { return Transform->GetScale(); }

				virtual void UpdateMatrix() { Transform->UpdateMatrix(); }
				virtual const glm::mat3& GetMatrix() { return Transform->GetMatrix(); }
				virtual const glm::mat3& GetUnscaledMatrix() { return Transform->GetUnscaledMatrix(); }
				virtual glm::vec2 Down() { return Transform->Down(); }
				virtual glm::vec2 Right() { return Transform->Right(); }

				virtual void AddChild(std::shared_ptr<Transform2DIF> child) {
					Transform->AddChild(child); 
				}
				virtual void RemoveChild(std::shared_ptr<Transform2DIF> child) { Transform->RemoveChild(child); }
				virtual std::shared_ptr<Transform2DIF> GetChild(size_t i) { return Transform->GetChild(i); }
				virtual size_t GetChildCount() { return Transform->GetChildCount(); }
				virtual std::shared_ptr<Transform2DIF> GetParent() { return Transform->GetParent(); }
				virtual void SetParent(std::shared_ptr<Transform2DIF> parent) { Transform->SetParent(parent); }
				
				// Additional Functionality
				std::shared_ptr<Core::Space::Transform2DIF> GetTransform() { return Transform; }
				virtual void Update() {
					UpdateMatrix();

					for (size_t i = 0; i < Transform->GetChildCount(); i++)
					{
						auto child = std::static_pointer_cast<Item>(Transform->GetChild(i));
						if (child != nullptr)
						{
							child->Update();
						}
					}
				}
				virtual void Draw(Core::Renderers::GuiRenderer* renderer) {
					for (size_t i = 0; i < Transform->GetChildCount(); i++)
					{
						auto child = std::static_pointer_cast<Item>(Transform->GetChild(i));
						if (child != nullptr)
						{
							child->Draw(renderer);
						}
					}
				}

				// Item subtype default behaviors - overridden in child classes where appropriate
				virtual void SetImage(std::string image) {}
				virtual void SetText(std::string text) {}
			};
		}
	}
}