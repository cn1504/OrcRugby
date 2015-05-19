#include "GameStateController.h"
#include <MainWindow.h>
#include <Components/Gui/Panel.h>
#include <Components/Gui/Anchored.h>
#include <Components/Gui/Label.h>
#include <Input/Map.h>
#include <Components/DayNightCycle.h>
#include <Components/Grid.h>
#include <Components/PrefsDB.h>

using namespace Game::Components;

GameStateController::GameStateController(Game::MainWindow* wh)
	: Window(wh)
{
	State = std::make_unique<InitialState>(this);
}
GameStateController::~GameStateController() {}

void GameStateController::LoadMainMenu()
{
	auto res = glm::vec2(Window->GetSize().x, Window->GetSize().y);
	auto guiScaleOffset = res / GUI_SIZE;
	
	LoadedGuiComponents.push_back(std::make_shared<Core::Components::Gui::Panel>("Background"));
	LoadedGuiComponents.back()->Scale(res);
	Window->AddGuiItemToLayer(0, LoadedGuiComponents.back());

	auto buttonPanel = std::make_shared<Core::Components::Gui::Panel>();
	buttonPanel->Scale(glm::vec2(865.0, 760.0) * guiScaleOffset);
	LoadedGuiComponents.push_back(std::make_shared<Core::Components::Gui::Anchored>(buttonPanel, std::make_unique<Core::Components::Gui::AlignBottomLeft>(glm::vec2(40, 40))));
	Window->AddGuiItemToLayer(1, LoadedGuiComponents.back());

	auto exitClickAction = std::make_shared<CloseAction>(this);
	auto exitButton = std::make_shared<Core::Components::Gui::Button>("Btn_ExitGame", exitClickAction);
	exitButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	exitButton->Translate(glm::vec2(0, -exitButton->GetScale().y * 4.0f));
	Window->AddButton(exitButton);
	buttonPanel->AddChild(exitButton);
	
	auto optionsClickAction = std::make_shared<Core::Input::NullAction>();
	auto optionsButton = std::make_shared<Core::Components::Gui::Button>("Btn_Options", optionsClickAction);
	optionsButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	optionsButton->Translate(glm::vec2(0, -optionsButton->GetScale().y * 2.0f));
	Window->AddButton(optionsButton);
	buttonPanel->AddChild(optionsButton);
	
	auto statisticsClickAction = std::make_shared<Core::Input::NullAction>();
	auto statisticsButton = std::make_shared<Core::Components::Gui::Button>("Btn_Statistics", statisticsClickAction);
	statisticsButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	Window->AddButton(statisticsButton);
	buttonPanel->AddChild(statisticsButton);
	
	auto newGameClickAction = std::make_shared<NewGameAction>(this);
	auto newGameButton = std::make_shared<Core::Components::Gui::Button>("Btn_NewGame", newGameClickAction);
	newGameButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	newGameButton->Translate(glm::vec2(0, newGameButton->GetScale().y * 2.0f));
	Window->AddButton(newGameButton);
	buttonPanel->AddChild(newGameButton);

	auto continueGameClickAction = std::make_shared<ContinueGameAction>(this);
	auto continueGameButton = std::make_shared<Core::Components::Gui::Button>("Btn_ContinueGame", continueGameClickAction);
	continueGameButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	continueGameButton->Translate(glm::vec2(0, continueGameButton->GetScale().y * 4.0f));
	Window->AddButton(continueGameButton);
	buttonPanel->AddChild(continueGameButton);
	if (Game::Save->GetInt("GameInProgress") == 0)
	{
		continueGameButton->SetState(std::make_unique<Core::Components::Gui::ButtonState>(continueGameButton.get()));
	}

	auto ver = Game::Prefs->GetString("Version");
	std::shared_ptr<Core::Components::Gui::Item> BuildText = std::make_shared<Core::Components::Gui::Label>("Consolas16", glm::vec4(1, 1, 1, 1), 1, ver);
	BuildText = std::make_shared<Core::Components::Gui::Anchored>(BuildText, std::make_unique<Core::Components::Gui::AlignBottomRight>(glm::vec2(6, 0)));
	Window->AddGuiItemToLayer(2, BuildText);
	LoadedGuiComponents.push_back(BuildText);

	// UI Keybinds
	EscAction = std::make_shared<CloseAction>(this);
	Window->InputMap->AddReleaseAction("Escape", EscAction);

	State = std::make_unique<MainMenuState>(this);
}
void GameStateController::LoadExistingGame()
{
	auto res = glm::vec2(Window->GetSize().x, Window->GetSize().y);
	auto guiScaleOffset = res / GUI_SIZE;

	auto DayDuration = Game::Save->GetFloat("TimeRatio");
	LoadedEntities.push_back(std::make_shared<Core::Components::DayNightCycle>(DayDuration));
	Core::Scene->AddChild(LoadedEntities.back());

	LoadedEntities.push_back(std::make_shared<Game::Components::Grid>());
	Core::Scene->AddChild(LoadedEntities.back());

	// UI Keybinds
	EscAction = std::make_shared<OpenGameMenuAction>(this);
	Window->InputMap->AddReleaseAction("Escape", EscAction);

	State = std::make_unique<InGameState>(this);
}
void GameStateController::LoadNewGame()
{
	// Initialize game state

	LoadExistingGame();
}

void GameStateController::CloseMainMenu()
{
	for (auto& i : LoadedGuiComponents)
	{
		i->GetParent()->RemoveChild(i);
	}
	LoadedGuiComponents.clear();	
	ActiveInputBindings.clear();
}
void GameStateController::CloseGame()
{
	CloseGameMenu();

	for (auto& i : LoadedGuiComponents)
	{
		i->GetParent()->RemoveChild(i);
	}
	LoadedGuiComponents.clear();
	ActiveInputBindings.clear();

	for (auto& i : LoadedEntities)
	{
		i->GetParent()->RemoveChild(i);
	}
	LoadedEntities.clear();
}

void GameStateController::OpenGameMenu()
{
	CloseInventory();
	CloseOptionsMenu();

	auto res = glm::vec2(Window->GetSize().x, Window->GetSize().y);
	auto guiScaleOffset = res / GUI_SIZE;

	auto buttonPanel = std::make_shared<Core::Components::Gui::Panel>("GameMenu_Background");
	buttonPanel->Scale(glm::vec2(1364, 1084) * guiScaleOffset);
	GameMenuComponents.push_back(buttonPanel);
	Window->AddGuiItemToLayer(3, GameMenuComponents.back());

	auto resumeClickAction = std::make_shared<CloseGameMenuAction>(this);
	auto resumeButton = std::make_shared<Core::Components::Gui::Button>("Btn_ResumeGame", resumeClickAction);
	resumeButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	resumeButton->Translate(glm::vec2(0, resumeButton->GetScale().y * 2.0f));
	Window->AddButton(resumeButton);
	buttonPanel->AddChild(resumeButton);

	auto optionsClickAction = std::make_shared<Core::Input::NullAction>();
	auto optionsButton = std::make_shared<Core::Components::Gui::Button>("Btn_Options", optionsClickAction);
	optionsButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	Window->AddButton(optionsButton);
	buttonPanel->AddChild(optionsButton);
	
	auto saveAndQuitClickAction = std::make_shared<CloseAction>(this);
	auto saveAndQuitButton = std::make_shared<Core::Components::Gui::Button>("Btn_SaveQuit", saveAndQuitClickAction);
	saveAndQuitButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	saveAndQuitButton->Translate(glm::vec2(0, -saveAndQuitButton->GetScale().y * 2.0f));
	Window->AddButton(saveAndQuitButton);
	buttonPanel->AddChild(saveAndQuitButton);
	
	// UI Keybinds
	EscAction = std::make_shared<CloseGameMenuAction>(this);
	Window->InputMap->AddReleaseAction("Escape", EscAction);
}
void GameStateController::CloseGameMenu()
{
	for (auto& i : GameMenuComponents)
	{
		i->GetParent()->RemoveChild(i);
	}
	GameMenuComponents.clear();

	// Reset UI Keybinds
	EscAction = std::make_shared<OpenGameMenuAction>(this);
	Window->InputMap->AddReleaseAction("Escape", EscAction);
}