#include "Button.h"

using namespace Core::Components::Gui;


Button::Button(std::string ButtonImage, std::shared_ptr<Core::Input::Action> ClickAction)
	: Panel(ButtonImage + "_Idle"), ButtonImage(ButtonImage), OnClickAction(ClickAction), State(std::make_unique<Idle>(this))
{
}
Button::~Button() {}
void Button::OnMouseOver() { State->OnMouseOver(); }
void Button::OnMouseOut() { State->OnMouseOut(); }
void Button::OnMouseDown() { State->OnMouseDown(); }
void Button::OnClick() { State->OnClick(); }

void Idle::OnMouseOver()
{
	button->SetImage(button->ButtonImage + "_Mouseover");
	button->State = std::make_unique<MouseOver>(button);
}

void MouseOver::OnMouseDown()
{
	button->SetImage(button->ButtonImage + "_Active");
	button->State = std::make_unique<MouseDown>(button);
}
void MouseOver::OnMouseOut()
{
	button->SetImage(button->ButtonImage + "_Idle");
	button->State = std::make_unique<Idle>(button);
}

void MouseDown::OnMouseOut()
{
	button->SetImage(button->ButtonImage + "_Idle");
	button->State = std::make_unique<Idle>(button);
}
void MouseDown::OnClick()
{
	auto clickAction = button->OnClickAction;
	button->SetImage(button->ButtonImage + "_Mouseover");
	button->State = std::make_unique<MouseOver>(button);
	clickAction->Perform();
}