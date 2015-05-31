#include "InGameState.h"
#include "MainMenuState.h"
#include <Components/Gui/Panel.h>
#include <Components/Gui/Anchored.h>
#include <Components/Gui/Label.h>
#include <Components/DayNightCycle.h>
#include <Components/Grid.h>
#include <Components/Hex.h>

#include <Components/Gui/LogPanel.h>

#include <Tests/GameLogSpammer.h>

using namespace Game::States;


InGameState::InGameState(GameStateController* GSC) : GameState(GSC)
{
	auto DayDuration = Game::Save->GetFloat("TimeRatio");
	LoadedEntities.push_back(std::make_shared<Core::Components::DayNightCycle>(DayDuration));
	Core::Scene->AddChild(LoadedEntities.back());

	auto Grid = std::make_shared<Game::Components::Grid>();
	LoadedEntities.push_back(Grid);
	Core::Scene->AddChild(Grid);

	// Reset Camera
	auto camera = GSC->Window->GetCamera();
	camera->Translate(glm::vec3(0, 12.313, -15.76) - camera->GetPosition());

	glm::vec3 direction = glm::normalize(glm::vec3(0, -0.615661f, 0.788011f));
	auto rot1 = glm::RotationBetweenVectors(camera->Forward(), direction);
	camera->Rotate(rot1);

	// Recompute desiredUp so that it's perpendicular to the direction
	// You can skip that part if you really want to force desiredUp
	glm::vec3 desiredUp(0.0, 1.0, 0.0);
	glm::vec3 right = glm::cross(camera->Forward(), desiredUp);
	desiredUp = glm::cross(right, camera->Forward());

	// Because of the 1st rotation, the up is probably completely screwed up. 
	// Find the rotation between the "up" of the rotated object, and the desired up
	camera->Rotate(glm::RotationBetweenVectors(camera->Up(), desiredUp));

	TogglePauseKeybind = std::make_shared<TogglePauseAction>(this);
	GSC->Window->InputMap->AddReleaseAction("Space", TogglePauseKeybind);

	ExpandGrid = std::make_shared<ExpandGridAction>(Grid, camera);
	GSC->Window->InputMap->AddReleaseAction("F", std::weak_ptr<Core::Input::Action>(ExpandGrid));

	// Testing tools
	auto gls = std::make_shared<Game::Tests::GameLogSpammer>();
	Core::Scene->AddChild(gls);

	LoadGUI();
}
InGameState::~InGameState()
{
	SaveGame();
	CloseGUI();
	CloseGameMenu();

	for (auto& i : LoadedEntities)
	{
		i->GetParent()->RemoveChild(i);
	}
	LoadedEntities.clear();
}


void InGameState::Reload()
{
	ReloadGUI();
}

void InGameState::LoadGUI()
{
	auto res = glm::vec2(GSC->Window->GetSize().x, GSC->Window->GetSize().y);
	auto guiScaleOffset = res / GSC->GUI_SIZE;

	auto CornerShade = std::make_shared<Core::Components::Gui::Panel>("InGame_BottomCornersShade");
	CornerShade->Scale(glm::vec2(3840, 2160) * guiScaleOffset);
	LoadedGuiComponents.push_back(CornerShade);
	GSC->Window->AddGuiItemToLayer(0, LoadedGuiComponents.back());

	// Center Panel
	std::shared_ptr<Core::Components::Gui::Item> centerPanel = std::make_shared<Core::Components::Gui::Panel>("InGame_Centre_Background");
	centerPanel->Scale(glm::vec2(812, 332) * guiScaleOffset);
	centerPanel = std::make_shared<Core::Components::Gui::Anchored>(centerPanel, std::make_unique<Core::Components::Gui::AlignBottom>(glm::vec2()));
	LoadedGuiComponents.push_back(centerPanel);
	GSC->Window->AddGuiItemToLayer(1, LoadedGuiComponents.back());
	
	auto inventoryAction = std::make_shared<Core::Input::NullAction>();
	auto inventoryButton = std::make_shared<Core::Components::Gui::Button>("InGame_Centre_InventoryButton", inventoryAction);
	inventoryButton->Scale(glm::vec2(214.0, 210.0) * guiScaleOffset);
	GSC->Window->AddButton(inventoryButton);
	centerPanel->AddChild(inventoryButton);
	inventoryButton->Translate(glm::vec2(-270.0, -60.0) * guiScaleOffset * 2.0f);

	auto gameMenuAction = std::make_shared<CloseAction>(GSC);
	auto gameMenuButton = std::make_shared<Core::Components::Gui::Button>("InGame_Centre_OptionsButton", gameMenuAction);
	gameMenuButton->Scale(glm::vec2(214.0, 210.0) * guiScaleOffset);
	GSC->Window->AddButton(gameMenuButton);
	centerPanel->AddChild(gameMenuButton);
	gameMenuButton->Translate(glm::vec2(290.0, -60.0) * guiScaleOffset * 2.0f);

	std::shared_ptr<Core::Components::Gui::Item> centerPanel2 = std::make_shared<Core::Components::Gui::Panel>();
	centerPanel2->Scale(glm::vec2(812, 84.0) * guiScaleOffset);
	centerPanel2 = std::make_shared<Core::Components::Gui::Anchored>(centerPanel2, std::make_unique<Core::Components::Gui::AlignBottom>(glm::vec2(16, 206.0) * guiScaleOffset));
	LoadedGuiComponents.push_back(centerPanel2);
	GSC->Window->AddGuiItemToLayer(2, LoadedGuiComponents.back());

	auto speedIncreaseAction = std::make_shared<IncreaseSpeedAction>(this);
	auto speedIncreaseButton = std::make_shared<Core::Components::Gui::Button>("InGame_Centre_SpeedIncreaseButton", speedIncreaseAction);
	speedIncreaseButton->Scale(glm::vec2(188.0, 84.0) * guiScaleOffset);
	GSC->Window->AddButton(speedIncreaseButton);
	centerPanel2->AddChild(speedIncreaseButton);
	speedIncreaseButton->Translate(glm::vec2(268.0, 0.0) * guiScaleOffset);

	auto speedDecreaseAction = std::make_shared<DecreaseSpeedAction>(this);
	auto speedDecreaseButton = std::make_shared<Core::Components::Gui::Button>("InGame_Centre_SpeedDecreaseButton", speedDecreaseAction);
	speedDecreaseButton->Scale(glm::vec2(188.0, 84.0) * guiScaleOffset);
	GSC->Window->AddButton(speedDecreaseButton);
	centerPanel2->AddChild(speedDecreaseButton);
	speedDecreaseButton->Translate(glm::vec2(-242.0, 0.0) * guiScaleOffset);

	auto speedSetting = std::make_shared<Core::Components::Gui::Panel>("InGame_Centre_SpeedSetting");
	speedSetting->Scale(glm::vec2(201.0, 130.0) * guiScaleOffset);
	centerPanel2->AddChild(speedSetting);
	speedSetting->Translate(glm::vec2(8.0, 20.0) * guiScaleOffset * 2.0f);

	SpeedPanel = std::make_shared<Core::Components::Gui::Panel>();
	SpeedPanel->Scale(glm::vec2(61.0, 38.0) * guiScaleOffset);
	centerPanel2->AddChild(SpeedPanel);
	SetSpeedState(std::make_unique<NormalSpeedState>(this));
	SpeedPanel->Translate(glm::vec2(8.0, 0.0) * guiScaleOffset * 2.0f);

	auto MoneyBox = std::make_shared<Core::Components::Gui::Panel>("InGame_Centre_MoneyBox");
	MoneyBox->Scale(glm::vec2(512, 164) * guiScaleOffset);
	centerPanel->AddChild(MoneyBox);
	MoneyBox->Translate(glm::vec2(12.0, -84.0) * guiScaleOffset * 2.0f);

	// Game / Combat Log
	std::shared_ptr<Core::Components::Gui::Item> logPanel = std::make_shared<Game::Components::Gui::LogPanel>();
	logPanel->Scale(glm::vec2(1200.0f, 200.0f) * guiScaleOffset);
	logPanel = std::make_shared<Core::Components::Gui::Anchored>(logPanel, std::make_unique<Core::Components::Gui::AlignBottomRight>(glm::vec2(20, 10)));
	LoadedGuiComponents.push_back(logPanel);
	GSC->Window->AddGuiItemToLayer(1, LoadedGuiComponents.back());
}
void InGameState::ReloadGUI()
{
	auto currentSpeedState = std::move(SpeedState);
	CloseGUI();
	LoadGUI();
	SetSpeedState(std::move(currentSpeedState));
}
void InGameState::CloseGUI()
{
	SpeedState = nullptr;
	SpeedPanel = nullptr;
	for (auto& i : LoadedGuiComponents)
	{
		i->GetParent()->RemoveChild(i);
	}
	LoadedGuiComponents.clear();
}


void InGameState::Close()
{
	if (GameMenuComponents.size() > 0)
	{
		CloseGameMenu();
	}
	else
	{
		OpenGameMenu();
	}
}
void InGameState::CloseGame()
{
	GSC->State = std::make_unique<MainMenuState>(GSC);
}
void CloseGameAction::Perform() { state->CloseGame(); }

void InGameState::OpenGameMenu()
{
	auto res = glm::vec2(GSC->Window->GetSize().x, GSC->Window->GetSize().y);
	auto guiScaleOffset = res / GSC->GUI_SIZE;

	auto buttonPanel = std::make_shared<Core::Components::Gui::Panel>("GameMenu_Background");
	buttonPanel->Scale(glm::vec2(1364, 1084) * guiScaleOffset);
	GameMenuComponents.push_back(buttonPanel);
	GSC->Window->AddGuiItemToLayer(3, GameMenuComponents.back());

	auto resumeClickAction = std::make_shared<CloseAction>(GSC);
	auto resumeButton = std::make_shared<Core::Components::Gui::Button>("Btn_ResumeGame", resumeClickAction);
	resumeButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	resumeButton->Translate(glm::vec2(0, resumeButton->GetScale().y * 2.0f));
	GSC->Window->AddButton(resumeButton);
	buttonPanel->AddChild(resumeButton);

	auto optionsClickAction = std::make_shared<Core::Input::NullAction>();
	auto optionsButton = std::make_shared<Core::Components::Gui::Button>("Btn_Options", optionsClickAction);
	optionsButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	GSC->Window->AddButton(optionsButton);
	buttonPanel->AddChild(optionsButton);

	auto saveAndQuitClickAction = std::make_shared<CloseGameAction>(this);
	auto saveAndQuitButton = std::make_shared<Core::Components::Gui::Button>("Btn_SaveQuit", saveAndQuitClickAction);
	saveAndQuitButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	saveAndQuitButton->Translate(glm::vec2(0, -saveAndQuitButton->GetScale().y * 2.0f));
	GSC->Window->AddButton(saveAndQuitButton);
	buttonPanel->AddChild(saveAndQuitButton);
}
void InGameState::CloseGameMenu()
{
	for (auto& i : GameMenuComponents)
	{
		i->GetParent()->RemoveChild(i);
	}
	GameMenuComponents.clear();
}


void InGameState::OpenInventory() {}
void InGameState::OpenEvent(std::string Event) {}
void InGameState::OpenOptionsMenu() {}


void InGameState::SaveGame()
{

}


void InGameState::SetSpeedState(std::unique_ptr<GameSpeedState> s)
{
	SpeedState = std::move(s);
	SpeedPanel->SetImage(SpeedState->GetPanelImage());
	Core::Time->Scale = SpeedState->GetTimeMultiplier();
}
void InGameState::TogglePause() { SpeedState->TogglePause(); }
void InGameState::IncreaseSpeed() { SpeedState->Increase(); }
void InGameState::DecreaseSpeed() { SpeedState->Decrease(); }
void TogglePauseAction::Perform() { state->TogglePause(); }
void IncreaseSpeedAction::Perform() { state->IncreaseSpeed(); }
void DecreaseSpeedAction::Perform() { state->DecreaseSpeed(); }
void GameSpeedState::TogglePause() { GS->SetSpeedState(std::make_unique<PausedState>(GS)); }

void PausedState::Increase() { GS->SetSpeedState(std::make_unique<NormalSpeedState>(GS)); }
void PausedState::TogglePause() { GS->SetSpeedState(std::move(PreviousState)); }
std::string PausedState::GetPanelImage() { return "InGame_Centre_Pause"; }
float PausedState::GetTimeMultiplier() { return 0.0f; }

void NormalSpeedState::Increase() { GS->SetSpeedState(std::make_unique<DoubleSpeedState>(GS)); }
void NormalSpeedState::Decrease() { GS->SetSpeedState(std::make_unique<PausedState>(GS)); }
std::string NormalSpeedState::GetPanelImage() { return "InGame_Centre_x1"; }
float NormalSpeedState::GetTimeMultiplier() { return 1.0f; }

void DoubleSpeedState::Increase() { GS->SetSpeedState(std::make_unique<QuadSpeedState>(GS)); }
void DoubleSpeedState::Decrease() { GS->SetSpeedState(std::make_unique<NormalSpeedState>(GS)); }
std::string DoubleSpeedState::GetPanelImage() { return "InGame_Centre_x2"; }
float DoubleSpeedState::GetTimeMultiplier() { return 2.0f; }

void QuadSpeedState::Decrease() { GS->SetSpeedState(std::make_unique<DoubleSpeedState>(GS)); }
std::string QuadSpeedState::GetPanelImage() { return "InGame_Centre_x4"; }
float QuadSpeedState::GetTimeMultiplier() { return 4.0f; }


ExpandGridAction::ExpandGridAction(std::weak_ptr<Game::Components::Grid> grid, std::weak_ptr<Core::Space::TransformIF> target)
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