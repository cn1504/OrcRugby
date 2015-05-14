#pragma once
#include "Item.h"
#include "Alignment.h"

namespace Core
{
	namespace Components
	{
		namespace Gui
		{
			// Anchored decorates Item
			class Anchored : public Item
			{
			protected:
				std::shared_ptr<Item> Target;
				std::unique_ptr<Alignment> AlignmentBehavior;

			public:
				Anchored() = delete;
				Anchored(const Anchored&) = delete;
				Anchored& operator=(const Anchored&) = delete;

				Anchored(std::shared_ptr<Item> target, std::unique_ptr<Alignment> alignment)
					: Target(target), AlignmentBehavior(std::move(alignment)) {
					Transform = Target->GetTransform();
				}
				virtual ~Anchored() {}
				
				virtual void Update() { AlignmentBehavior->Realign(Target); Target->Update(); }
				virtual void Draw(Core::Renderers::GuiRenderer* renderer) {
					Target->Draw(renderer);
				}
				virtual void SetImage(std::string image) { Target->SetImage(image); }
				virtual void SetText(std::string text) { Target->SetText(text); }
			};
		}
	}
}