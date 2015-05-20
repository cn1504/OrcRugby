#include "InGameState.h"
#include "MainMenuState.h"
#include <Components/Gui/Panel.h>
#include <Components/Gui/Anchored.h>
#include <Components/Gui/Label.h>
#include <Components/DayNightCycle.h>
#include <Components/Grid.h>

using namespace Game::States;


InGameState::InGameState(GameStateController* GSC) : GameState(GSC)
{
	auto DayDuration = Game::Save->GetFloat("TimeRatio");
	LoadedEntities.push_back(std::make_shared<Core::Components::DayNightCycle>(DayDuration));
	Core::Scene->AddChild(LoadedEntities.back());

	LoadedEntities.push_back(std::make_shared<Game::Components::Grid>());
	Core::Scene->AddChild(LoadedEntities.back());

	// Reset Camera Position
	auto camera = GSC->Window->GetCamera();
	camera->Translate(glm::vec3(0, 20, -20) - camera->GetPosition());
	camera->Rotate(glm::RotationBetweenVectors(camera->Forward(), glm::vec3(0, glm::sqrt(2.0f) / 2.0f, glm::sqrt(2.0f) / 2.0f)));

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

	auto speedIncreaseAction = std::make_shared<Core::Input::NullAction>();
	auto speedIncreaseButton = std::make_shared<Core::Components::Gui::Button>("InGame_Centre_SpeedIncreaseButton", speedIncreaseAction);
	speedIncreaseButton->Scale(glm::vec2(188.0, 84.0) * guiScaleOffset);
	GSC->Window->AddButton(speedIncreaseButton);
	centerPanel2->AddChild(speedIncreaseButton);
	speedIncreaseButton->Translate(glm::vec2(268.0, 0.0) * guiScaleOffset);

	auto speedDecreaseAction = std::make_shared<Core::Input::NullAction>();
	auto speedDecreaseButton = std::make_shared<Core::Components::Gui::Button>("InGame_Centre_SpeedDecreaseButton", speedDecreaseAction);
	speedDecreaseButton->Scale(glm::vec2(188.0, 84.0) * guiScaleOffset);
	GSC->Window->AddButton(speedDecreaseButton);
	centerPanel2->AddChild(speedDecreaseButton);
	speedDecreaseButton->Translate(glm::vec2(-242.0, 0.0) * guiScaleOffset);

	auto speedSetting = std::make_shared<Core::Components::Gui::Panel>("InGame_Centre_SpeedSetting");
	speedSetting->Scale(glm::vec2(201.0, 130.0) * guiScaleOffset);
	centerPanel2->AddChild(speedSetting);
	speedSetting->Translate(glm::vec2(8.0, 20.0) * guiScaleOffset * 2.0f);

	

	auto MoneyBox = std::make_shared<Core::Components::Gui::Panel>("InGame_Centre_MoneyBox");
	MoneyBox->Scale(glm::vec2(512, 164) * guiScaleOffset);
	centerPanel->AddChild(MoneyBox);
	MoneyBox->Translate(glm::vec2(12.0, -84.0) * guiScaleOffset * 2.0f);

}
void InGameState::ReloadGUI()
{
	CloseGUI();
	LoadGUI();
}
void InGameState::CloseGUI()
{
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