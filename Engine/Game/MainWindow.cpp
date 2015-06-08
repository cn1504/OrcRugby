#include "MainWindow.h"
#include <Input/Map.h>
#include <Components/Gui/Panel.h>
#include <Components/Gui/Label.h>
#include <Components/Gui/Anchored.h>
#include <Components/StaticMesh.h>
#include <Components/LightSource.h>
#include <Components/DayNightCycle.h>
#include <Assets/AssetDB.h>
#include <Assets/Texture.h>
#include <Assets/Font.h>

#include <Components/Grid.h>
#include <Components/Hex.h>
#include <Components/PrefsDB.h>
#include <States/GameStateController.h>

#include <Components/Characters/Character.h>

using namespace Game;
using namespace Core::Window;

const int GUI_LAYERS = 5;

MainWindow::MainWindow() 
	: DefaultWindow("Game", 
		glm::ivec2(Game::Prefs->GetInt("MainWindow_PosX"), Game::Prefs->GetInt("MainWindow_PosY")),
		glm::ivec2(Game::Prefs->GetInt("MainWindow_Width"), Game::Prefs->GetInt("MainWindow_Height")))
{
	for (int i = 0; i < GUI_LAYERS; i++)
	{
		auto layer = std::make_shared<Core::Components::Gui::Panel>();
		layer->Scale(Gui->GetScale());
		Gui->AddChild(layer);
		Layers.push_back(layer);
	}

	FPSLabel = std::make_shared<Core::Components::Gui::Label>("Consolas16", glm::vec4(1, 1, 1, 1), 1, "FPS: 0");
	FPSLabel = std::make_shared<Core::Components::Gui::Anchored>(FPSLabel, std::make_unique<Core::Components::Gui::AlignBottomLeft>(glm::vec2(6,0)));
	AddGuiItemToLayer(static_cast<int>(Layers.size() - 1), FPSLabel);

	//Camera->Translate(glm::vec3(0, 10, -17.32));
	//Camera->Rotate(glm::vec3(30.0, 0, 0));

	if (Core::Window::Map.size() < 2)
	{
		// Character Test
		Core::Debug->Log("");
		auto chara = Game::Generator->GenerateCharacter();
		Core::Debug->Log(std::to_string(*chara));
		Core::Debug->Log("");
		chara = Game::Generator->GenerateCharacter();
		Core::Debug->Log(std::to_string(*chara));
		Core::Debug->Log("");
		chara = Game::Generator->GenerateCharacter();
		Core::Debug->Log(std::to_string(*chara));
	}

	GSC = std::make_shared<Game::States::GameStateController>(this);
	GSC->MainMenu();

	OnMouseLeftDown = std::make_shared<ClickDownAction>(this);
	InputMap->AddPressAction("Mouse Left", OnMouseLeftDown);
	OnMouseLeftRelease = std::make_shared<ClickAction>(this);
	InputMap->AddReleaseAction("Mouse Left", OnMouseLeftRelease);

	// Register camera controls
	CameraUp = std::make_shared<CameraUpAction>(std::weak_ptr<Core::Space::TransformIF>(Camera), 2.0f, true);
	InputMap->AddWhileDownAction(Game::Prefs->GetString("CameraUp"), std::weak_ptr<Core::Input::Action>(CameraUp));
	CameraDown = std::make_shared<CameraDownAction>(std::weak_ptr<Core::Space::TransformIF>(Camera), 2.0f, true);
	InputMap->AddWhileDownAction(Game::Prefs->GetString("CameraDown"), std::weak_ptr<Core::Input::Action>(CameraDown));
	CameraLeft = std::make_shared<CameraLeftAction>(std::weak_ptr<Core::Space::TransformIF>(Camera), 2.0f, true);
	InputMap->AddWhileDownAction(Game::Prefs->GetString("CameraLeft"), std::weak_ptr<Core::Input::Action>(CameraLeft));
	CameraRight = std::make_shared<CameraRightAction>(std::weak_ptr<Core::Space::TransformIF>(Camera), 2.0f, true);
	InputMap->AddWhileDownAction(Game::Prefs->GetString("CameraRight"), std::weak_ptr<Core::Input::Action>(CameraRight));
	CameraOut = std::make_shared<CameraZoomInAction>(std::weak_ptr<Core::Space::TransformIF>(Camera), 60.0f, true);
	InputMap->AddReleaseAction(Game::Prefs->GetString("CameraOut"), std::weak_ptr<Core::Input::Action>(CameraOut));
	CameraIn = std::make_shared<CameraZoomOutAction>(std::weak_ptr<Core::Space::TransformIF>(Camera), 60.0f, true);
	InputMap->AddReleaseAction(Game::Prefs->GetString("CameraIn"), std::weak_ptr<Core::Input::Action>(CameraIn));
	CameraRotateLeft = std::make_shared<CameraRotationAction>(std::weak_ptr<Core::Space::TransformIF>(Camera), -0.5f);
	InputMap->AddWhileDownAction(Game::Prefs->GetString("CameraRotateLeft"), std::weak_ptr<Core::Input::Action>(CameraRotateLeft));
	CameraRotateRight = std::make_shared<CameraRotationAction>(std::weak_ptr<Core::Space::TransformIF>(Camera), 0.5f);
	InputMap->AddWhileDownAction(Game::Prefs->GetString("CameraRotateRight"), std::weak_ptr<Core::Input::Action>(CameraRotateRight));

}
MainWindow::~MainWindow() {}
int MainWindow::Update()
{
	BeginUpdate();

	FPSLabel->SetText("FPS: " + std::to_string((int)Core::Time->FPS));
	
	// Update Button States
	bool deactivate = false;
	auto mp = InputMap->MousePosition;
	mp.y = Size.y - mp.y;
	for (size_t i = Layers.size(); i > 0; i--)
	{
		bool result = false;
		for (size_t j = 0; j < Layers[i - 1]->GetChildCount(); j++)
		{
			bool r = UpdateButtons(Layers[i - 1]->GetChild(j), mp, deactivate);
			if (r == true)
			{
				result = true;
			}
		}

		if (result == true)
		{
			deactivate = true;
		}
	}

	return DefaultWindow::Update();
}

bool MainWindow::UpdateButtons(std::shared_ptr<Core::Space::Transform2DIF> parent, const glm::vec2& mp, bool deactivate)
{
	bool r = false;

	auto p = glm::vec2(parent->GetMatrix() * glm::vec3(0, 0, 1));
	p = glm::vec2(p.x * Size.x / 2.0f + Size.x / 2.0f, p.y * Size.y / 2.0f + Size.y / 2.0f);
	auto s = parent->GetScale();

	if ((mp.x > p.x - s.x / 2.0f && mp.x < p.x + s.x / 2.0f)
		&& (mp.y > p.y - s.y / 2.0f && mp.y < p.y + s.y / 2.0f))
	{
		auto b = std::dynamic_pointer_cast<Core::Components::Gui::Button>(parent);
		if (b)
		{
			if (deactivate)
				b->OnMouseOut();
			else
				b->OnMouseOver();
		}
		r = true;
	}
	else
	{
		auto b = std::dynamic_pointer_cast<Core::Components::Gui::Button>(parent);
		if (b)
			b->OnMouseOut();		
	}

	for (size_t i = 0; i < parent->GetChildCount(); i++)
	{
		auto c = parent->GetChild(i);
		auto r2 = UpdateButtons(c, mp, deactivate);
		if (r2)
			r = true;
	}

	return r;
}

void MainWindow::Translate(const glm::ivec2& delta)
{
	DefaultWindow::Translate(delta);

	Game::Prefs->Set("MainWindow_PosX", Position.x);
	Game::Prefs->Set("MainWindow_PosY", Position.y);
}
void MainWindow::Scale(const glm::ivec2& delta)
{
	DefaultWindow::Scale(delta);

	if (Size.x != 0 && Size.y != 0)
	{
		Game::Prefs->Set("MainWindow_Width", Size.x);
		Game::Prefs->Set("MainWindow_Height", Size.y);

		for (auto& l : Layers)
		{
			l->Scale(Gui->GetScale() / l->GetScale());
		}

		GSC->Reload();
	}
}
void MainWindow::AddGuiItemToLayer(int i, std::shared_ptr<Core::Components::Gui::Item> item)
{
	if (i < Layers.size())
	{
		Layers[i]->AddChild(item);
	}
}
void MainWindow::AddButton(std::shared_ptr<Core::Components::Gui::Button> button)
{
	Buttons.push_back(button);
}


NewGameWindowAction::~NewGameWindowAction() {}
void NewGameWindowAction::Perform()
{
	new Game::MainWindow();
}


void ClickAction::Perform()
{
	for (size_t i = 0; i < Window->Buttons.size(); i++)
	{
		if (auto b = Window->Buttons[i].lock())
		{
			b->OnClick();
		}
		else
		{
			std::swap(Window->Buttons[i], Window->Buttons.back());
			Window->Buttons.pop_back();
			i--;
		}
	}
}

void ClickDownAction::Perform()
{
	for (size_t i = 0; i < Window->Buttons.size(); i++)
	{
		if (auto b = Window->Buttons[i].lock())
		{
			b->OnMouseDown();
		}
		else
		{
			std::swap(Window->Buttons[i], Window->Buttons.back());
			Window->Buttons.pop_back();
			i--;
		}
	}
}