#include "GameStateController.h"

using namespace Game::Components;

GameStateController::GameStateController(Game::MainWindow* wh)
	: Window(wh)
{
	State = std::make_unique<InitialState>();
}
GameStateController::~GameStateController();

void GameStateController::MainMenu();
void GameStateController::ContinueGame();
void GameStateController::NewGame();
void GameStateController::OpenGameMenu();
void GameStateController::OpenInventory();
void GameStateController::OpenEvent();