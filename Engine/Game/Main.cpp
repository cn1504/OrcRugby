#include <Game.h>
#include "MainWindow.h"
#include <Input/Input.h>
#include <Assets/Database.h>
#include <Assets/SQLiteDB.h>

#include <Components/PrefsDB.h>
#include <Components/SaveDB.h>

// Global Game Objects
std::shared_ptr<Game::Components::PrefsDB> Game::Prefs;
std::shared_ptr<Game::Components::SaveDB> Game::Save;


int main(int argc, char* argv[])
{
	try {
		Core::Init(argc, argv);

		auto prefsdb = std::make_shared<Core::Assets::SQLiteDB>("prefs.dat");
		Game::Prefs = std::make_shared<Game::Components::PrefsDB>(prefsdb);

		auto savedb = std::make_shared<Core::Assets::SQLiteDB>("save.dat");
		Game::Save = std::make_shared<Game::Components::SaveDB>(savedb);

		// Open the main window
		auto createWindowAction = std::make_shared<Game::NewGameWindowAction>();
		Core::Input::GlobalMap.AddReleaseAction("Ctrl + Alt + N", std::weak_ptr<Core::Input::Action>(createWindowAction));
		createWindowAction->Perform();
		
		Core::Run();
	}
	catch (std::runtime_error &e) {
		Core::Debug->Log(e.what());
		Core::Close();
		exit(EXIT_FAILURE);
	}
	return 0;
}