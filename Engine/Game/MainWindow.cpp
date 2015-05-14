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

using namespace Game;
using namespace Core::Window;

MainWindow::MainWindow() 
	: DefaultWindow("Game", 
		glm::ivec2(Game::Prefs->GetInt("MainWindow_PosX"), Game::Prefs->GetInt("MainWindow_PosY")),
		glm::ivec2(Game::Prefs->GetInt("MainWindow_Width"), Game::Prefs->GetInt("MainWindow_Height")))
{
	FPSLabel = std::make_shared<Core::Components::Gui::Label>("Consolas16", glm::vec4(1, 1, 1, 1), 1, "FPS: 0");
	FPSLabel = std::make_shared<Core::Components::Gui::Anchored>(FPSLabel, std::make_unique<Core::Components::Gui::AlignBottomLeft>(glm::vec2(6,0)));
	
	Camera->Translate(glm::vec3(0, 20, -20));
	Camera->Rotate(glm::vec3(45.0, 0, 0));

	if (Core::Window::Map.size() < 2)
	{
		/*
		auto dnc = std::make_shared<Core::Components::DayNightCycle>(1440.0f);
		Core::Scene->AddChild(dnc);
		
		auto gb = std::make_shared<Game::Components::Grid>();
		Core::Scene->AddChild(gb);
		*/

		GSC = std::make_shared<Game::Components::GameStateController>(this);

		// Load Main Menu
		auto res = Gui->GetScale();
		MMBackground = std::make_shared<Core::Components::Gui::Panel>("Background");
		MMBackground->Scale(Gui->GetScale());
		Gui->AddChild(MMBackground);
	}
	Gui->AddChild(FPSLabel);

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

	return DefaultWindow::Update();
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

		MMBackground->Scale(Gui->GetScale() / MMBackground->GetScale());
	}
}


NewGameWindowAction::~NewGameWindowAction() {}
void NewGameWindowAction::Perform()
{
	new Game::MainWindow();
}
