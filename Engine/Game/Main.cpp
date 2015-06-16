#include <Game.h>
#include "MainWindow.h"
#include <Input/Input.h>
#include <Assets/Database.h>
#include <Assets/SQLiteDB.h>

#include <Components/PrefsDB.h>
#include <Components/SaveDB.h>
#include <Components/Log.h>
#include <Components/Generator.h>

#include <Audio/Listener.h>

#include <Components/Characters/Character.h>

// Global Game Objects
std::shared_ptr<Game::Components::PrefsDB> Game::Prefs;
std::shared_ptr<Game::Components::SaveDB> Game::Save;
std::shared_ptr<Game::Components::Log> Game::Log;
std::shared_ptr<Game::Components::Generator> Game::Generator;
std::shared_ptr<Core::Components::AudioSource> Game::BGM;

std::unordered_map<std::string, std::vector<std::shared_ptr<Game::Components::Characters::Character>>> Game::Factions;

int main(int argc, char* argv[])
{
	try {
		Core::Init(argc, argv);

		auto prefsdb = std::make_shared<Core::Assets::SQLiteDB>("prefs.dat");
		Game::Prefs = std::make_shared<Game::Components::PrefsDB>(prefsdb);

		auto savedb = std::make_shared<Core::Assets::SQLiteDB>("save.dat");
		Game::Save = std::make_shared<Game::Components::SaveDB>(savedb);

		auto gameLog = std::make_shared<Game::Components::Log>();
		Game::Log = gameLog;

		Game::Generator = std::make_unique<Game::Components::Generator>(Core::Database);

		// Load Settings and Preferences
		if (Game::Prefs->GetInt("AudioEnabled") == 0)
			Core::Listener->ToggleAudio();
		if (Game::Prefs->GetInt("MusicEnabled") == 0)
			Core::Listener->ToggleMusic();

		Core::Listener->SetVolume(Game::Prefs->GetFloat("MainVolume"));
		Core::Listener->SetMusicVolume(Game::Prefs->GetFloat("MusicVolume"));
		Core::Listener->SetAmbientVolume(Game::Prefs->GetFloat("AmbientVolume"));
		Core::Listener->SetSFXVolume(Game::Prefs->GetFloat("SFXVolume"));
		Core::Listener->SetVoiceVolume(Game::Prefs->GetFloat("VoiceVolume"));

		Core::Debug->Log("");

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