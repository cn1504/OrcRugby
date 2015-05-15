#pragma once

#include "Panel.h"
#include <Input/Action.h>

namespace Core
{
	namespace Components
	{
		namespace Gui
		{
			class ButtonState;
			class Idle;
			class MouseOver;
			class MouseDown;

			class Button : public Panel
			{
				friend class Idle;
				friend class MouseOver;
				friend class MouseDown;

			protected:
				std::string ButtonImage;
				std::shared_ptr<Core::Input::Action> OnClickAction;
				std::unique_ptr<ButtonState> State;

			public:
				Button() = delete;
				Button(const Button&) = delete;
				Button& operator=(const Button&) = delete;

				Button(std::string ButtonImage, std::shared_ptr<Core::Input::Action> ClickAction);
				virtual ~Button();

				// Button State Transitions
				void OnMouseOver();
				void OnMouseOut();
				void OnMouseDown();
				void OnClick();
			};

			class ButtonState
			{
			protected:
				Button* button;

			public:
				ButtonState(Button* button) : button(button) {}

				virtual void OnMouseOver() {}
				virtual void OnMouseOut() {}
				virtual void OnMouseDown() {}
				virtual void OnClick() {}
			};

			class Idle : public ButtonState
			{
			public:
				Idle(Button* button) : ButtonState(button) {}
				virtual void OnMouseOver();
			};
			class MouseOver : public ButtonState
			{
			public:
				MouseOver(Button* button) : ButtonState(button) {}
				virtual void OnMouseDown();
				virtual void OnMouseOut();
			};
			class MouseDown : public ButtonState
			{
			public:
				MouseDown(Button* button) : ButtonState(button) {}
				virtual void OnMouseOut();
				virtual void OnClick();
			};
		}
	}
}