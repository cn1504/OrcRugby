#include "GameStateController.h"
#include <MainWindow.h>
#include <Components/Gui/Panel.h>
#include <Components/Gui/Anchored.h>
#include <Components/Gui/Label.h>
#include <Input/Map.h>
#include <Components/DayNightCycle.h>
#include <Components/Grid.h>

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
	LoadedGuiComponents.push_back(std::make_shared<Core::Components::Gui::Panel>("Background"));
	LoadedGuiComponents.back()->Scale(res);
	Window->AddGuiItemToLayer(0, LoadedGuiComponents.back());

	ActiveInputBindings.push_back(std::make_shared<CloseAction>(this));
	Window->InputMap->AddReleaseAction("Escape", ActiveInputBindings.back());

	auto guiScaleOffset = res / glm::vec2(3840.0, 2160.0);
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
	
	auto newGameClickAction = std::make_shared<Core::Input::NullAction>();
	auto newGameButton = std::make_shared<Core::Components::Gui::Button>("Btn_NewGame", newGameClickAction);
	newGameButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	newGameButton->Translate(glm::vec2(0, newGameButton->GetScale().y * 2.0f));
	Window->AddButton(newGameButton);
	buttonPanel->AddChild(newGameButton);

	auto continueGameClickAction = std::make_shared<Core::Input::NullAction>();
	auto continueGameButton = std::make_shared<Core::Components::Gui::Button>("Btn_ContinueGame", continueGameClickAction);
	continueGameButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	continueGameButton->Translate(glm::vec2(0, continueGameButton->GetScale().y * 4.0f));
	Window->AddButton(continueGameButton);
	buttonPanel->AddChild(continueGameButton);

	std::shared_ptr<Core::Components::Gui::Item> BuildText = std::make_shared<Core::Components::Gui::Label>("Consolas16", glm::vec4(1, 1, 1, 1), 1, "0.1.14");
	BuildText = std::make_shared<Core::Components::Gui::Anchored>(BuildText, std::make_unique<Core::Components::Gui::AlignBottomRight>(glm::vec2(6, 0)));
	Window->AddGuiItemToLayer(2, BuildText);

	State = std::make_unique<MainMenuState>(this);
}
void GameStateController::LoadExistingGame()
{
	auto dnc = std::make_shared<Core::Components::DayNightCycle>(1440.0f);
	Core::Scene->AddChild(dnc);

	auto gb = std::make_shared<Game::Components::Grid>();
	Core::Scene->AddChild(gb);
}
void GameStateController::LoadNewGame()
{
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
}