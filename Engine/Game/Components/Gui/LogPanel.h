#pragma once

#include <Game.h>
#include <Components/Gui/Panel.h>
#include <Components/Gui/Label.h>

namespace Game
{
	namespace Components
	{
		namespace Gui
		{
			class LogPanel : public Core::Components::Gui::Panel, public Core::Patterns::Observer
			{
			private:
				std::shared_ptr<Core::Components::Gui::Item> Text;
				size_t LinePosition;
				size_t LinesDisplayed;	
				size_t CharsPerRow;

				bool ObserverRegistered;

				void UpdateText();

			public:
				LogPanel();
				LogPanel(std::string background);
				virtual ~LogPanel();

				virtual void Notify();
				virtual void Scale(const glm::vec2& delta);
				virtual void Update();
			};
		}
	}
}