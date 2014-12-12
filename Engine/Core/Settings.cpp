#include "Settings.h"

namespace Core
{
	namespace Settings
	{
		namespace Window
		{
			int X;
			int Y;
			int Width;
			int Height;
			bool Fullscreen;

			int DefaultWidth;
			int DefaultHeight;
		}

		namespace Video {
			float FOVY;
			float MinDrawDistance;
			float MaxDrawDistance;

			int FXAA;

			bool VSync;
			float MaxForegroundFPS;
			float MaxBackgroundFPS;

			bool ShowFPS;

			int ShadowResolution;
			int LightmapResolution;
		}

		namespace Audio
		{
			int AudioEnabled;
			int MusicEnabled;

			float MainVolume;
			float MusicVolume;
			float SoundEffectsVolume;
			float AmbientEffectsVolume;
			float VoiceVolume;
		}

		namespace Game
		{
		}

		namespace Controls
		{
			float CameraRotationSpeed;
			float MouseRotationSensitivity;
		}

		namespace Misc
		{
			int VerboseLogging; 
			bool ShowConsole;
		}

		namespace
		{
			std::string settingsFilename = "settings.cfg";
			std::fstream settingsFile;
		}

		void Init(int argc, char* argv[])
		{
			// Initialize to default settings
			Window::Width = 1280;
			Window::Height = 720;
			Window::DefaultWidth = 1280;
			Window::DefaultHeight = 720;
			Window::X = (glfwGetVideoMode(glfwGetPrimaryMonitor())->width - Window::Width) / 2;
			Window::Y = (glfwGetVideoMode(glfwGetPrimaryMonitor())->height - Window::Height) / 2;
			Window::Fullscreen = false;

			Video::FOVY = 45;
			Video::MinDrawDistance = 0.1f;
			Video::MaxDrawDistance = 1000;
			Video::FXAA = 1;
			Video::VSync = true;
			Video::MaxForegroundFPS = 120;
			Video::MaxBackgroundFPS = 15;
			Video::ShowFPS = true;
			Video::ShadowResolution = 256;
			Video::LightmapResolution = 256;

			Audio::AudioEnabled = 1;
			Audio::MusicEnabled = 1;			
			Audio::MainVolume = 1.0f;
			Audio::MusicVolume = 1.0f;
			Audio::SoundEffectsVolume = 0.8f;
			Audio::AmbientEffectsVolume = 0.8f;
			Audio::VoiceVolume = 1.0f;
			
			Controls::CameraRotationSpeed = 90.0f;	// Degrees Per Second
			Controls::MouseRotationSensitivity = 0.25f;

			Misc::VerboseLogging = 1;
			Misc::ShowConsole = 1;

			// Read settings file
			settingsFile.open(settingsFilename, std::ios::in);
			if (settingsFile)
			{
				std::string setting;
				std::string value;
				do {
					getline(settingsFile, setting, ':');
					getline(settingsFile, value);

					if (setting.length() < 1 || value.length() < 1)
					{

					}
					else if (setting == "WindowWidth") { Window::Width = std::stoi(value); }
					else if (setting == "WindowHeight") { Window::Height = std::stoi(value); }
					else if (setting == "WindowPositionX") { Window::X = std::stoi(value); }
					else if (setting == "WindowPositionY") { Window::Y = std::stoi(value); }
					else if (setting == "DefaultWindowWidth") { Window::DefaultWidth = std::stoi(value); }
					else if (setting == "DefaultWindowHeight") { Window::DefaultHeight = std::stoi(value); }
					else if (setting == "Fullscreen") { Window::Fullscreen = (std::stoi(value) > 0); }

					else if (setting == "FOVY") { Video::FOVY = std::stof(value); }
					else if (setting == "MinDrawDistance") { Video::MinDrawDistance = std::stof(value); }
					else if (setting == "MaxDrawDistance") { Video::MaxDrawDistance = std::stof(value); }
					else if (setting == "FXAA") { Video::FXAA = std::stoi(value); }
					else if (setting == "VSync") { Video::VSync = (std::stoi(value) > 0); }
					else if (setting == "MaxForegroundFPS") { Video::MaxForegroundFPS = std::stof(value); }
					else if (setting == "MaxBackgroundFPS") { Video::MaxBackgroundFPS = std::stof(value); }
					else if (setting == "ShowFPS") { Video::ShowFPS = (std::stoi(value) > 0); }
					else if (setting == "ShadowResolution") { Video::ShadowResolution = std::stoi(value); }
					else if (setting == "LightmapResolution") { Video::LightmapResolution = std::stoi(value); }

					else if (setting == "AudioEnabled") { Audio::AudioEnabled = std::stoi(value); }
					else if (setting == "MusicEnabled") { Audio::MusicEnabled = std::stoi(value); }
					else if (setting == "MainVolume") { Audio::MainVolume = std::stof(value); }
					else if (setting == "MusicVolume") { Audio::MusicVolume = std::stof(value); }
					else if (setting == "SoundEffectsVolume") { Audio::SoundEffectsVolume = std::stof(value); }
					else if (setting == "AmbientEffectsVolume") { Audio::AmbientEffectsVolume = std::stof(value); }
					else if (setting == "VoiceVolume") { Audio::VoiceVolume = std::stof(value); }

					else if (setting == "CameraRotationSpeed") { Controls::CameraRotationSpeed = std::stof(value); }
					else if (setting == "MouseRotationSensitivity") { Controls::MouseRotationSensitivity = std::stof(value); }
					
					else if (setting == "VerboseLogging") { Misc::VerboseLogging = std::stoi(value); }
					else if (setting == "ShowConsole") { Misc::ShowConsole = (std::stoi(value) > 0); }

					setting = "";
					value = "";
				} while (!settingsFile.fail());
				settingsFile.close();
			}

			// Process command line args
			for (int i = 1; i < argc; i++)
			{
				std::string arg(argv[i]);
				if (arg == "-v") { Misc::VerboseLogging = 1; }
				else if (arg == "-ns") { Audio::AudioEnabled = 0; Audio::MusicEnabled = 0; }
			}
		}

		void Close()
		{
			// Save current settings to settings file
			settingsFile.open(settingsFilename, std::ios::out);
			if (settingsFile)
			{
				settingsFile << "WindowWidth" << ": " << Window::Width << std::endl;
				settingsFile << "WindowHeight" << ": " << Window::Height << std::endl;
				settingsFile << "WindowPositionX" << ": " << Window::X << std::endl;
				settingsFile << "WindowPositionY" << ": " << Window::Y << std::endl;
				settingsFile << "DefaultWindowWidth" << ": " << Window::DefaultWidth << std::endl;
				settingsFile << "DefaultWindowHeight" << ": " << Window::DefaultHeight << std::endl;
				settingsFile << "Fullscreen" << ": " << Window::Fullscreen << std::endl;

				settingsFile << "FOVY" << ": " << Video::FOVY << std::endl;
				settingsFile << "MinDrawDistance" << ": " << Video::MinDrawDistance << std::endl;
				settingsFile << "MaxDrawDistance" << ": " << Video::MaxDrawDistance << std::endl;
				settingsFile << "FXAA" << ": " << Video::FXAA << std::endl;
				settingsFile << "VSync" << ": " << Video::VSync << std::endl;
				settingsFile << "MaxForegroundFPS" << ": " << Video::MaxForegroundFPS << std::endl;
				settingsFile << "MaxBackgroundFPS" << ": " << Video::MaxBackgroundFPS << std::endl;
				settingsFile << "ShowFPS" << ": " << Video::ShowFPS << std::endl;
				settingsFile << "ShadowResolution" << ": " << Video::ShadowResolution << std::endl;
				settingsFile << "LightmapResolution" << ": " << Video::LightmapResolution << std::endl;

				settingsFile << "AudioEnabled" << ": " << Audio::AudioEnabled << std::endl;
				settingsFile << "MusicEnabled" << ": " << Audio::MusicEnabled << std::endl;
				settingsFile << "MainVolume" << ": " << Audio::MainVolume << std::endl;
				settingsFile << "MusicVolume" << ": " << Audio::MusicVolume << std::endl;
				settingsFile << "SoundEffectsVolume" << ": " << Audio::SoundEffectsVolume << std::endl;
				settingsFile << "AmbientEffectsVolume" << ": " << Audio::AmbientEffectsVolume << std::endl;
				settingsFile << "VoiceVolume" << ": " << Audio::VoiceVolume << std::endl;
				
				settingsFile << "CameraRotationSpeed" << ": " << Controls::CameraRotationSpeed << std::endl;
				settingsFile << "MouseRotationSensitivity" << ": " << Controls::MouseRotationSensitivity << std::endl;

				settingsFile << "VerboseLogging" << ": " << Misc::VerboseLogging << std::endl;
				settingsFile << "ShowConsole" << ": " << Misc::ShowConsole << std::endl;
			}
			else
			{
				Debug::Log("Invalid settings file.");
			}
		}
	}
}