#pragma once
#include "Core.h"
#include "Scene.h"
#include "Entity.h"

namespace Core
{
	class Input
	{
	public:
		enum class CameraMode { Game, Orbital };
		enum class InputMode { TextInput, KBM, Joystick };
		enum class MovementMode { Walk, Run, Dodge, Idle };

	private:
		Window* Window;
		GLFWwindow* WindowPtr;
		int currentMonitor;
		int monitorCount;
		GLFWmonitor** monitors;

		Entity* Camera;
		Entity* Target;
		Entity* Character;

		CameraMode cameraMode;
		float orbitalCameraDistance;
		float orbitalCameraLatitude;
		float orbitalCameraLongitude;
		float gameCameraLongitude;
		glm::vec2 MousePosition;			// Click Event Position
		glm::vec2 CurrentMousePosition;		// Update Position

		void RepositionWindow();

		std::string consoleOutput;
		Gui::Item* Console;
		Gui::Item* FPSDisplay;
		bool consoleShown;

		InputMode inputMode;
		Gui::TextBox* TextInputItem;
		std::string TextInput;
		std::string OriginalTextInput;

		glm::vec2 CalculateMousePosition();
		glm::vec3 CalculateMouseWorldSpaceVector();

		MovementMode moveType;
		glm::vec2 moveDirection;
		glm::vec2 mouseMoveDirection;
		std::unordered_set<std::string> Actions;

	public:
		Input(Core::Window* window, GLFWwindow* ptr);
		~Input();

		void Update();
		void KeyEvent(int key, int scancode, int action, int mods);
		void MouseButtonEvent(int button, int action, int mods);
		void ScrollEvent(double x, double y);

		void SetTargetEntity(Entity* target);
		void SetCameraEntity(Entity* camera);
		void SetCharacterEntity(Entity* character);
		Entity* GetCharacterEntity();
		void SetConsole(Gui::Item* console);
		void SetFPSDisplay(Gui::Item* item);

		void ToggleFullscreen();
		void SetCameraMode(CameraMode mode);
		void StopCharacter();

		void ActivateTextInput(Gui::TextBox* textInputBox);
	};
}