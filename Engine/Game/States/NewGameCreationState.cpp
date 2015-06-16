#include "NewGameCreationState.h"
#include "MainMenuState.h"
#include "InGameState.h"

using namespace Game::States;


NewGameCreationState::NewGameCreationState(GameStateController* GSC) : GameState(GSC)
{
	auto res = glm::vec2(GSC->Window->GetSize().x, GSC->Window->GetSize().y);
	auto guiScaleOffset = res / GSC->GUI_SIZE;

	Game::BGM->FadeToNewFile("Silence", 1.0f);

	Game::Save->NewGame();
}
NewGameCreationState::~NewGameCreationState()
{
}
void NewGameCreationState::Close() { GSC->State = std::make_unique<MainMenuState>(GSC); }