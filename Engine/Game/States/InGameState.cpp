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
	auto res = glm::vec2(GSC->Window->GetSize().x, GSC->Window->GetSize().y);
	auto guiScaleOffset = res / GSC->GUI_SIZE;

	auto DayDuration = Game::Save->GetFloat("TimeRatio");
	LoadedEntities.push_back(std::make_shared<Core::Components::DayNightCycle>(DayDuration));
	Core::Scene->AddChild(LoadedEntities.back());

	LoadedEntities.push_back(std::make_shared<Game::Components::Grid>());
	Core::Scene->AddChild(LoadedEntities.back());
}
InGameState::~InGameState()
{
	for (auto& i : LoadedGuiComponents)
	{
		i->GetParent()->RemoveChild(i);
	}
	LoadedGuiComponents.clear();

	for (auto& i : GameMenuComponents)
	{
		i->GetParent()->RemoveChild(i);
	}
	GameMenuComponents.clear();

	for (auto& i : LoadedEntities)
	{
		i->GetParent()->RemoveChild(i);
	}
	LoadedEntities.clear();
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
