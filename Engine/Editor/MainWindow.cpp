#include "MainWindow.h"
#include <Input/Input.h>
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

#include "Grid.h"
#include "Hex.h"

using namespace Editor;
using namespace Core::Window;

MainWindow::MainWindow() : DefaultWindow("Editor", glm::ivec2((1920-1280)/2, (1080-720)/2), glm::ivec2(1280, 720)) 
{
	//Core::AssetDB->SetTexture("Consolas32", 512, 512);
	//Core::AssetDB->SetTexture("Sky_R", 1024, 1024);
	//Core::Assets::Font f("Consolas32", 32, 32, 16, 0, 0);
	//Core::AssetDB->SetFont("Consolas32", f);

	auto Background = std::make_shared<Core::Components::Gui::Panel>();
	Background->Scale(glm::vec2(1024, 1024));
	Gui->AddChild(Background);

	FPSLabel = std::make_shared<Core::Components::Gui::Label>("Consolas32", glm::vec4(1, 1, 1, 1), 1, "FPS: 0");
	FPSLabel = std::make_shared<Core::Components::Gui::Anchored>(FPSLabel, std::make_unique<Core::Components::Gui::AlignBottomLeft>(glm::vec2()));
	Gui->AddChild(FPSLabel); 
	
	Camera->Translate(glm::vec3(0,0,-10));

	if (Core::Window::Map.size() < 2)
	{
		auto cube = std::make_shared<Core::Components::StaticMesh>("Cube", "Plastic_White");
		cube->Translate(glm::vec3(5, 0.5, 0));
		Core::Scene->AddChild(cube);
		
		auto dnc = std::make_shared<Core::Components::DayNightCycle>(1440.0f);
		Core::Scene->AddChild(dnc);

		auto gb = std::make_shared<Editor::Components::Grid>();
		Core::Scene->AddChild(gb);

		ExpandGrid = std::make_shared<ExpandGridAction>(gb, Camera);
		Core::Input::GlobalMap.AddReleaseAction("F", std::weak_ptr<Core::Input::Action>(ExpandGrid));
	}

	CameraFoward = std::make_shared<EntityMovementFowardAction>(std::weak_ptr<Core::Space::TransformIF>(Camera), 1.0f, true);
	InputMap->AddWhileDownAction("W", std::weak_ptr<Core::Input::Action>(CameraFoward));
	CameraBack = std::make_shared<EntityMovementBackAction>(std::weak_ptr<Core::Space::TransformIF>(Camera), 1.0f, true);
	InputMap->AddWhileDownAction("S", std::weak_ptr<Core::Input::Action>(CameraBack));
	CameraLeft = std::make_shared<EntityMovementLeftAction>(std::weak_ptr<Core::Space::TransformIF>(Camera), 1.0f, true);
	InputMap->AddWhileDownAction("A", std::weak_ptr<Core::Input::Action>(CameraLeft));
	CameraRight = std::make_shared<EntityMovementRightAction>(std::weak_ptr<Core::Space::TransformIF>(Camera), 1.0f, true);
	InputMap->AddWhileDownAction("D", std::weak_ptr<Core::Input::Action>(CameraRight));
	CameraUp = std::make_shared<EntityMovementUpAction>(std::weak_ptr<Core::Space::TransformIF>(Camera), 1.0f, true);
	InputMap->AddWhileDownAction("Space", std::weak_ptr<Core::Input::Action>(CameraUp));
	CameraDown = std::make_shared<EntityMovementDownAction>(std::weak_ptr<Core::Space::TransformIF>(Camera), 1.0f, true);
	InputMap->AddWhileDownAction("Left Shift", std::weak_ptr<Core::Input::Action>(CameraDown));

	CameraMouseLook = std::make_shared<CameraRotationAction>(std::weak_ptr<Core::Space::TransformIF>(Camera), &(InputMap->MouseDelta), 1.0f);
	InputMap->AddWhileDownAction("Mouse Right", std::weak_ptr<Core::Input::Action>(CameraMouseLook));
}
MainWindow::~MainWindow() {}
int MainWindow::Update()
{
	BeginUpdate();

	FPSLabel->SetText("FPS: " + std::to_string((int)Core::Time->FPS));

	return DefaultWindow::Update();
}


NewEditorWindowAction::~NewEditorWindowAction() {}
void NewEditorWindowAction::Perform() 
{
	new Editor::MainWindow();
}



EntityMovementAction::EntityMovementAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime)
{
	Target = std::move(target);
	Velocity = velocity;
	InRealtime = inRealtime;
}
EntityMovementAction::~EntityMovementAction() {}
void EntityMovementAction::Perform()
{
	if (auto a = Target.lock())
	{
		a->Translate(Direction(a) * Velocity * (InRealtime ? Core::Time->RealtimeDelta : Core::Time->Delta));
	}
	else
	{
		delete this;
	}
}


EntityMovementUpAction::EntityMovementUpAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime)
	: EntityMovementAction(target, velocity, inRealtime) {}
EntityMovementUpAction::~EntityMovementUpAction() {}
glm::vec3 EntityMovementUpAction::Direction(std::shared_ptr<Core::Space::TransformIF>& t) { return t->Up(); }

EntityMovementDownAction::EntityMovementDownAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime)
	: EntityMovementAction(target, velocity, inRealtime) {}
EntityMovementDownAction::~EntityMovementDownAction() {}
glm::vec3 EntityMovementDownAction::Direction(std::shared_ptr<Core::Space::TransformIF>& t) { return -t->Up(); }

EntityMovementLeftAction::EntityMovementLeftAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime)
	: EntityMovementAction(target, velocity, inRealtime) {}
EntityMovementLeftAction::~EntityMovementLeftAction() {}
glm::vec3 EntityMovementLeftAction::Direction(std::shared_ptr<Core::Space::TransformIF>& t) { return -t->Right(); }

EntityMovementRightAction::EntityMovementRightAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime)
	: EntityMovementAction(target, velocity, inRealtime) {}
EntityMovementRightAction::~EntityMovementRightAction() {}
glm::vec3 EntityMovementRightAction::Direction(std::shared_ptr<Core::Space::TransformIF>& t) { return t->Right(); }

EntityMovementFowardAction::EntityMovementFowardAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime)
	: EntityMovementAction(target, velocity, inRealtime) {}
EntityMovementFowardAction::~EntityMovementFowardAction() {}
glm::vec3 EntityMovementFowardAction::Direction(std::shared_ptr<Core::Space::TransformIF>& t) { return t->Forward(); }

EntityMovementBackAction::EntityMovementBackAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime)
	: EntityMovementAction(target, velocity, inRealtime) {}
EntityMovementBackAction::~EntityMovementBackAction() {}
glm::vec3 EntityMovementBackAction::Direction(std::shared_ptr<Core::Space::TransformIF>& t) { return -t->Forward(); }


CameraRotationAction::CameraRotationAction(std::weak_ptr<Core::Space::TransformIF> target, const glm::vec2* mouseDelta, float sensitivity)
{
	MouseDelta = mouseDelta;
	Sensitivity = sensitivity;
	Target = target;
}
CameraRotationAction::~CameraRotationAction() {}
void CameraRotationAction::Perform()
{
	if (auto a = Target.lock())
	{
		a->Rotate(glm::angleAxis(-(*MouseDelta).x * Sensitivity * 0.1f, a->Up()));
		a->Rotate(glm::angleAxis(-(*MouseDelta).y * Sensitivity * 0.1f, a->Right()));
	}
	else
	{
		delete this;
	}
}


ItemTranslationAction::ItemTranslationAction(std::weak_ptr<Core::Space::Translatable2D> target, const glm::vec2& velocity)
{
	Target = std::move(target);
	Velocity = velocity;
}
ItemTranslationAction::~ItemTranslationAction() {}
void ItemTranslationAction::Perform()
{
	if (auto a = Target.lock())
	{
		a->Translate(Velocity * Core::Time->RealtimeDelta);
	}
	else
	{
		delete this;
	}
}


ItemRotationAction::ItemRotationAction(std::weak_ptr<Core::Space::Rotatable2D> target, float velocity)
{
	Target = std::move(target);
	Velocity = velocity;
}
ItemRotationAction::~ItemRotationAction() {}
void ItemRotationAction::Perform()
{
	if (auto a = Target.lock())
	{
		a->Rotate(Velocity * Core::Time->RealtimeDelta);
	}
	else
	{
		delete this;
	}
}


EntityRotateLeftAction::EntityRotateLeftAction(std::weak_ptr<Core::Space::TransformIF> target, float angularVelocity)
{
	Target = target;
	AngularVelocity = angularVelocity;
}
EntityRotateLeftAction::~EntityRotateLeftAction() {}
void EntityRotateLeftAction::Perform()
{
	if (auto a = Target.lock())
	{
		a->Rotate(glm::vec3(0, AngularVelocity * Core::Time->RealtimeDelta, 0));
	}
	else
	{
		delete this;
	}
}


ExpandGridAction::ExpandGridAction(std::weak_ptr<Editor::Components::Grid> grid, std::weak_ptr<Core::Space::TransformIF> target)
{
	Grid = grid;
	Target = target;
}
ExpandGridAction::~ExpandGridAction() {}
void ExpandGridAction::Perform()
{
	if (auto t = Target.lock())
	{
		if (auto g = Grid.lock())
		{
			auto pos = glm::vec3(t->GetMatrix() * glm::vec4(0, 0, 0, 1));
			auto cHex = g->FindClosestHex(pos);
			cHex->Expand();
		}
	}
}