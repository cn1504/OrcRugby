#include "MainMenuState.h"
#include <Components/Gui/Panel.h>
#include <Components/Gui/Anchored.h>
#include <Components/Gui/Label.h>
#include <Components/PrefsDB.h>

#include "InGameState.h"
#include "NewGameCreationState.h"

using namespace Game::States;



MainMenuState::MainMenuState(GameStateController* GSC) : GameState(GSC)
{
	Load();
}
MainMenuState::~MainMenuState()
{
	Cleanup();
}
void MainMenuState::ContinueGame() { GSC->State = std::make_unique<InGameState>(GSC); }
void MainMenuState::NewGame() { GSC->State = std::make_unique<NewGameCreationState>(GSC); }
void MainMenuState::Close() { GSC->Window->Close(); }


void MainMenuState::Load()
{
	auto res = glm::vec2(GSC->Window->GetSize().x, GSC->Window->GetSize().y);
	auto guiScaleOffset = res / GSC->GUI_SIZE;

	LoadedGuiComponents.push_back(std::make_shared<Core::Components::Gui::Panel>("Background"));
	LoadedGuiComponents.back()->Scale(res);
	GSC->Window->AddGuiItemToLayer(0, LoadedGuiComponents.back());

	auto buttonPanel = std::make_shared<Core::Components::Gui::Panel>();
	buttonPanel->Scale(glm::vec2(865.0, 760.0) * guiScaleOffset);
	LoadedGuiComponents.push_back(std::make_shared<Core::Components::Gui::Anchored>(buttonPanel, std::make_unique<Core::Components::Gui::AlignBottomLeft>(glm::vec2(40, 40))));
	GSC->Window->AddGuiItemToLayer(1, LoadedGuiComponents.back());

	auto exitClickAction = std::make_shared<CloseAction>(GSC);
	auto exitButton = std::make_shared<Core::Components::Gui::Button>("Btn_ExitGame", exitClickAction);
	exitButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	exitButton->Translate(glm::vec2(0, -exitButton->GetScale().y * 4.0f));
	GSC->Window->AddButton(exitButton);
	buttonPanel->AddChild(exitButton);

	auto optionsClickAction = std::make_shared<Core::Input::NullAction>();
	auto optionsButton = std::make_shared<Core::Components::Gui::Button>("Btn_Options", optionsClickAction);
	optionsButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	optionsButton->Translate(glm::vec2(0, -optionsButton->GetScale().y * 2.0f));
	GSC->Window->AddButton(optionsButton);
	buttonPanel->AddChild(optionsButton);

	auto statisticsClickAction = std::make_shared<Core::Input::NullAction>();
	auto statisticsButton = std::make_shared<Core::Components::Gui::Button>("Btn_Statistics", statisticsClickAction);
	statisticsButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	GSC->Window->AddButton(statisticsButton);
	buttonPanel->AddChild(statisticsButton);

	auto newGameClickAction = std::make_shared<NewGameAction>(GSC);
	auto newGameButton = std::make_shared<Core::Components::Gui::Button>("Btn_NewGame", newGameClickAction);
	newGameButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	newGameButton->Translate(glm::vec2(0, newGameButton->GetScale().y * 2.0f));
	GSC->Window->AddButton(newGameButton);
	buttonPanel->AddChild(newGameButton);

	auto continueGameClickAction = std::make_shared<ContinueGameAction>(GSC);
	auto continueGameButton = std::make_shared<Core::Components::Gui::Button>("Btn_ContinueGame", continueGameClickAction);
	continueGameButton->Scale(glm::vec2(865.0, 150) * guiScaleOffset);
	continueGameButton->Translate(glm::vec2(0, continueGameButton->GetScale().y * 4.0f));
	GSC->Window->AddButton(continueGameButton);
	buttonPanel->AddChild(continueGameButton);
	if (Game::Save->GetInt("GameInProgress") == 0)
	{
		continueGameButton->SetState(std::make_unique<Core::Components::Gui::ButtonState>(continueGameButton.get()));
	}

	auto ver = Game::Prefs->GetString("Version");
	std::shared_ptr<Core::Components::Gui::Item> BuildText = std::make_shared<Core::Components::Gui::Label>("Consolas13", glm::vec4(1, 1, 1, 1), 1, ver);
	BuildText = std::make_shared<Core::Components::Gui::Anchored>(BuildText, std::make_unique<Core::Components::Gui::AlignBottomRight>(glm::vec2(6, 0)));
	GSC->Window->AddGuiItemToLayer(2, BuildText);
	LoadedGuiComponents.push_back(BuildText);

	Game::BGM->FadeToNewFile("AcousticLoop1", 1.0f);
}
void MainMenuState::Cleanup()
{
	for (auto& i : LoadedGuiComponents)
	{
		i->GetParent()->RemoveChild(i);
	}
	LoadedGuiComponents.clear();
}
void MainMenuState::Reload()
{
	Cleanup();
	Load();
}