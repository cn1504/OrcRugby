#include "GameStateController.h"
#include <MainWindow.h>
#include <Components/Gui/Panel.h>
#include <Components/Gui/Anchored.h>
#include <Components/Gui/Label.h>
#include <Input/Map.h>
#include "MainMenuState.h"

using namespace Game::States;

GameStateController::GameStateController(Game::MainWindow* wh)
	: Window(wh)
{
	State = std::make_unique<InitialState>(this);
	EscAction = std::make_shared<CloseAction>(this);
	Window->InputMap->AddReleaseAction("Escape", EscAction);
}
GameStateController::~GameStateController() {}


void InitialState::MainMenu() { GSC->State = std::make_unique<MainMenuState>(GSC); }


