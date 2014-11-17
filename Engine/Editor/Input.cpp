#include "Input.h"
#include "Window.h"
#include "Assets.h"
#include "SkeletalAnimation.h"
#include "Character.h"
#include "Camera.h"

#define GAME_CAMERA_LATITUDE (45.0f)
#define GAME_CAMERA_DISTANCE_MIN (10.0f)
#define GAME_CAMERA_DISTANCE_MAX (14.0f)
#define JOYSTICK_DEADZONE (0.2f)
#define JOYSTICK_RUNZONE (0.6f)
#define JOYSTICK_CAMERA_ROTATION_SPEED (90.0f)	// Degrees per second

namespace Core
{
	Input::Input(Core::Window* window, GLFWwindow* ptr)
	{
		Window = window;
		WindowPtr = ptr;

		cameraMode = CameraMode::Orbital;
		orbitalCameraDistance = 3.0f;
		orbitalCameraLatitude = 60.0f;
		orbitalCameraLongitude = 0.0f;
		gameCameraLongitude = 0.0f;

		int currentMonitor = 0;
		monitors = glfwGetMonitors(&monitorCount);
		GLFWmonitor* prime = glfwGetPrimaryMonitor();
		for (currentMonitor = 0; currentMonitor < monitorCount; currentMonitor++)
		{
			if (prime == monitors[currentMonitor])
				break;
		}

		Character = nullptr;
		consoleShown = Settings::Misc::ShowConsole;
		inputMode = InputMode::KBM;
		moveType = MovementMode::Run;
	}

	Input::~Input()
	{

	}

	void Input::Update()
	{
		double x, y;
		glfwGetCursorPos(WindowPtr, &x, &y);
		CurrentMousePosition = glm::vec2(x, y);

		Window->Scene->Gui->Update(CurrentMousePosition);

		if (inputMode == InputMode::TextInput)
		{
			// Text position indicator?
			return;
		}

		if (cameraMode == CameraMode::Game)
		{
			moveDirection.x = 0.0f;
			moveDirection.y = 0.0f;
			mouseMoveDirection.x = 0.0f;
			mouseMoveDirection.y = 0.0f;
			Actions.clear();

			if (inputMode == InputMode::KBM)
			{
				if (glfwGetMouseButton(WindowPtr, GLFW_MOUSE_BUTTON_2))
				{
					// Free mouse rotation 
					glm::vec2 deltaP = CurrentMousePosition - MousePosition;
					gameCameraLongitude += deltaP.x * Settings::Controls::MouseRotationSensitivity;
					MousePosition = CurrentMousePosition;
				}

				if (glfwGetMouseButton(WindowPtr, GLFW_MOUSE_BUTTON_1))
				{
					if (Character != nullptr)
					{
						if (glfwGetKey(WindowPtr, GLFW_KEY_LEFT_SHIFT))
						{
							Actions.insert("Primary");
						}
						else
						{
							glm::vec3 hitPoint;
							Entity* hitEntity;
							auto hit = Window->Scene->PhysicsWorld->RayCastHitPoint(Camera->Transform.Position, CalculateMouseWorldSpaceVector(), hitPoint, &hitEntity);
							if (hit)
							{
								glm::vec3 moveVector = hitPoint - Character->Transform.Position;
								mouseMoveDirection = glm::normalize(glm::vec2(moveVector.x, moveVector.z));

								if (hitEntity->GetComponent<Core::Character>() != nullptr)
								{
									Debug::Log("Clicked on a character.");
									if (hitEntity != Character)
									{
										Actions.insert("Primary");
									}
									else
									{
										mouseMoveDirection.x = 0.0f;
										mouseMoveDirection.y = 0.0f;
									}
								}
							}
						}
					}
				}


				if (glfwGetKey(WindowPtr, GLFW_KEY_W))
				{
					moveDirection.y += 1.0f;
				}
				if (glfwGetKey(WindowPtr, GLFW_KEY_A))
				{
					moveDirection.x += 1.0f;
				}
				if (glfwGetKey(WindowPtr, GLFW_KEY_S))
				{
					moveDirection.y -= 1.0f;
				}
				if (glfwGetKey(WindowPtr, GLFW_KEY_D))
				{
					moveDirection.x -= 1.0f;
				}
				if (glfwGetKey(WindowPtr, GLFW_KEY_F))
				{
					Actions.insert("Secondary");
				}

				if (glfwGetKey(WindowPtr, GLFW_KEY_Q))
				{
					Actions.insert("Dodge");
				}
			}

			// Joystick input
			int JoystickCount = 0;
			while (glfwJoystickPresent(JoystickCount) == GL_TRUE)
				JoystickCount++;
			for (int i = 0; i < JoystickCount; i++)
			{
				int jBCount;
				auto jButtons = glfwGetJoystickButtons(i, &jBCount);
				for (int b = 0; b < jBCount; b++)
				{
					if (jButtons[b] == GLFW_PRESS)
					{
						if (inputMode == InputMode::KBM)
						{
							Window->Scene->AppendConsole("Joystick input mode activated.");
							inputMode = InputMode::Joystick;
						}

						// Map input to actions									   XB360
						if (b == GLFW_JOYSTICK_1)								// A
						{
							Actions.insert("Dodge");
						}
						else if (b == GLFW_JOYSTICK_2)							// B
						{
						}
						else if (b == GLFW_JOYSTICK_3)							// X
						{
						}
						else if (b == GLFW_JOYSTICK_4)							// Y
						{
						}
						else if (b == GLFW_JOYSTICK_5)							// LB
						{
						}
						else if (b == GLFW_JOYSTICK_6)							// RB
						{
						}
						else if (b == GLFW_JOYSTICK_7)							// Back
						{
						}
						else if (b == GLFW_JOYSTICK_8)							// Start
						{
						}
						else if (b == GLFW_JOYSTICK_9)							// LH
						{
						}
						else if (b == GLFW_JOYSTICK_10)							// RH
						{
						}
						else if (b == GLFW_JOYSTICK_11)							// PAD_UP
						{
						}
						else if (b == GLFW_JOYSTICK_12)							// PAD_RIGHT
						{
						}
						else if (b == GLFW_JOYSTICK_13)							// PAD_DOWN
						{
						}
						else if (b == GLFW_JOYSTICK_14)							// PAD_LEFT
						{
						}
					}
				}
			}
			if (inputMode == InputMode::Joystick)
			{
				for (int i = 0; i < JoystickCount; i++)
				{
					int jACount;
					auto jAxes = glfwGetJoystickAxes(i, &jACount);
					for (int a = 0; a < jACount; a++)
					{
						if (jAxes[a] > JOYSTICK_DEADZONE || jAxes[a] < -JOYSTICK_DEADZONE)
						{
							if (a == 0)											// LS, LR
							{
								moveDirection.x -= jAxes[a];
							}
							else if (a == 1)									// LS, DU
							{
								moveDirection.y += jAxes[a];
							}
							else if (a == 2)
							{
								if (jAxes[a] > JOYSTICK_DEADZONE)				// LT
								{
									Actions.insert("Secondary");
								}
								else if (jAxes[a] < -JOYSTICK_DEADZONE) 		// RT
								{
									Actions.insert("Primary");
								}
							}
							else if (a == 3)									// RS, UD
							{
							}
							else if (a == 4)									// RS, LR
							{
								gameCameraLongitude += jAxes[a] * Time::RealtimeDelta * JOYSTICK_CAMERA_ROTATION_SPEED;
							}
						}
					}
				}

				moveType = (glm::length(moveDirection) > JOYSTICK_RUNZONE) ? MovementMode::Run : MovementMode::Walk;
			}

			// Update the camera position
			// Normalize angle
			while (gameCameraLongitude > 180.0f)
				gameCameraLongitude -= 360.0f;
			while (gameCameraLongitude < -180.0f)
				gameCameraLongitude += 360.0f;

			// Calculate camera position
			//float t = glm::distance(CurrentMousePosition, glm::vec2(Settings::Window::Width, Settings::Window::Height) * 0.5f) / (Settings::Window::Width * 0.5f);
			//t = (t < 0.5f) ? 0 : (t - 0.5f) * 2.0f;
			float distance = GAME_CAMERA_DISTANCE_MIN; //glm::mix(GAME_CAMERA_DISTANCE_MIN, GAME_CAMERA_DISTANCE_MAX, t);
			glm::vec3 position;
			position.x = Target->Transform.Position.x + distance*glm::cos(glm::radians(gameCameraLongitude))*glm::cos(glm::radians(GAME_CAMERA_LATITUDE));
			position.y = Target->Transform.Position.y + distance*                                            glm::sin(glm::radians(GAME_CAMERA_LATITUDE));
			position.z = Target->Transform.Position.z + distance*glm::sin(glm::radians(gameCameraLongitude))*glm::cos(glm::radians(GAME_CAMERA_LATITUDE));
			position.y += 1.0f;
			Camera->Transform.Position = position;
			Camera->Transform.Forward = glm::normalize(Target->Transform.Position - position);


			// Process input
			auto c = Character->GetComponent<Core::Character>();
			if (c->IsControlEnabled())
			{
				if (Actions.count("Dodge") > 0)
				{
					if (inputMode == InputMode::KBM)
					{
						auto mp = CalculateMousePosition();
						c->Dodge(glm::normalize(mp - glm::vec2(Character->Transform.Position.x, Character->Transform.Position.z)));
					}
					else
					{
						auto md = Character->Transform.WSForward();
						c->Dodge(glm::vec2(md.x, md.z));
					}
				}
				else if (Actions.count("Primary") > 0)
				{
					if (inputMode == InputMode::KBM)
					{
						auto mp = CalculateMousePosition();
						c->PrimaryAttack(glm::normalize(mp - glm::vec2(Character->Transform.Position.x, Character->Transform.Position.z)));
					}
					else
					{
						auto md = Character->Transform.WSForward();
						c->PrimaryAttack(glm::vec2(md.x, md.z));
					}
				}
				else if (Actions.count("Secondary") > 0)
				{
					if (inputMode == InputMode::KBM)
					{
						auto mp = CalculateMousePosition();
						c->SecondaryAttack(glm::normalize(mp - glm::vec2(Character->Transform.Position.x, Character->Transform.Position.z)));
					}
					else
					{
						auto md = Character->Transform.WSForward();
						c->SecondaryAttack(glm::vec2(md.x, md.z));
					}
				}
				else
				{
					// Calculate world space move direction and move the character
					glm::vec2 md;
					if (glm::length(moveDirection) > JOYSTICK_DEADZONE)
					{
						auto cf3 = Target->Transform.Position - Camera->Transform.Position;
						cf3.y = 0.0f;
						auto rot = RotationBetweenVectors(glm::vec3(0, 0, 1), cf3);

						auto md3 = rot * glm::normalize(glm::vec3(moveDirection.x, 0.0f, moveDirection.y));
						md += glm::vec2(md3.x, md3.z);
					}
					if (glm::length(mouseMoveDirection) > JOYSTICK_DEADZONE)
					{
						md += mouseMoveDirection;
					}

					if (glm::length(md) > JOYSTICK_DEADZONE)
					{
						c->Move(moveType, glm::normalize(md));
					}
					else
					{
						c->StopMoving();
					}
				}
			}
		}

		if (cameraMode == CameraMode::Orbital)
		{
			if (glfwGetMouseButton(WindowPtr, GLFW_MOUSE_BUTTON_2))
			{
				// Free mouse rotation 
				glm::vec2 deltaP = CurrentMousePosition - MousePosition;
				orbitalCameraLongitude += deltaP.x * Settings::Controls::MouseRotationSensitivity;
				orbitalCameraLatitude += deltaP.y * Settings::Controls::MouseRotationSensitivity;
				MousePosition = CurrentMousePosition;

				orbitalCameraLatitude = glm::clamp(orbitalCameraLatitude, -89.0f, 89.0f);
			}

			// Update the camera position
			// Normalize angle
			while (orbitalCameraLongitude > 180.0f)
				orbitalCameraLongitude -= 360.0f;
			while (orbitalCameraLongitude < -180.0f)
				orbitalCameraLongitude += 360.0f;

			// Calculate camera position
			float distance = orbitalCameraDistance; 
			glm::vec3 position;
			position.x = Target->Transform.Position.x + distance*glm::cos(glm::radians(orbitalCameraLongitude))*glm::cos(glm::radians(orbitalCameraLatitude));
			position.y = Target->Transform.Position.y + distance*                                               glm::sin(glm::radians(orbitalCameraLatitude));
			position.z = Target->Transform.Position.z + distance*glm::sin(glm::radians(orbitalCameraLongitude))*glm::cos(glm::radians(orbitalCameraLatitude));
			position.y += 1.0f;
			Camera->Transform.Position = position;
			Camera->Transform.Forward = glm::normalize(Target->Transform.Position - position);
		}
	}


	void Input::KeyEvent(int key, int scancode, int action, int mods)
	{
		if (inputMode == InputMode::TextInput)
		{
			if (action == GLFW_PRESS)
			{
				if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER)
				{
					Window->Scene->AppendConsole("KBM input mode activated.");
					inputMode = InputMode::KBM;
					TextInputItem->SaveValue();
					return;
				}
				else if (key == GLFW_KEY_BACKSPACE && TextInput.size() != 0)
				{
					TextInput.resize(TextInput.size() - 1);
				}
				else if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_9)
				{
					TextInput.push_back(key - 272);
				}
				else if (key == GLFW_KEY_KP_DECIMAL)	{ TextInput.push_back('.'); }
				else if (key == GLFW_KEY_KP_ADD)		{ TextInput.push_back('+'); }
				else if (key == GLFW_KEY_KP_SUBTRACT)	{ TextInput.push_back('-'); }
				else if (key == GLFW_KEY_KP_MULTIPLY)	{ TextInput.push_back('*'); }
				else if (key == GLFW_KEY_KP_DIVIDE)		{ TextInput.push_back('/'); }
				else if (GLFW_MOD_SHIFT & mods)
				{
					if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z)				TextInput.push_back(key);
					else if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9)		TextInput.push_back(key - 16);
					else if (key == GLFW_KEY_0)								TextInput.push_back(')');
					else if (key == GLFW_KEY_MINUS)							TextInput.push_back('_');
					else if (key == GLFW_KEY_EQUAL)							TextInput.push_back('+');
					else if (key == GLFW_KEY_LEFT_BRACKET)					TextInput.push_back('{');
					else if (key == GLFW_KEY_RIGHT_BRACKET)					TextInput.push_back('}');
					else if (key == GLFW_KEY_BACKSLASH)						TextInput.push_back('|');
					else if (key == GLFW_KEY_SEMICOLON)						TextInput.push_back(':');
					else if (key == GLFW_KEY_APOSTROPHE)					TextInput.push_back('"');
					else if (key == GLFW_KEY_COMMA)							TextInput.push_back('<');
					else if (key == GLFW_KEY_PERIOD)						TextInput.push_back('>');
					else if (key == GLFW_KEY_SLASH)							TextInput.push_back('/');
				}
				else
				{
					if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z)				TextInput.push_back(key + 32);
					else if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9)		TextInput.push_back(key);
					else if (key == GLFW_KEY_MINUS)							TextInput.push_back('-');
					else if (key == GLFW_KEY_EQUAL)							TextInput.push_back('=');
					else if (key == GLFW_KEY_LEFT_BRACKET)					TextInput.push_back('[');
					else if (key == GLFW_KEY_RIGHT_BRACKET)					TextInput.push_back(']');
					else if (key == GLFW_KEY_BACKSLASH)						TextInput.push_back('\\');
					else if (key == GLFW_KEY_SEMICOLON)						TextInput.push_back(';');
					else if (key == GLFW_KEY_APOSTROPHE)					TextInput.push_back('\'');
					else if (key == GLFW_KEY_COMMA)							TextInput.push_back(',');
					else if (key == GLFW_KEY_PERIOD)						TextInput.push_back('.');
					else if (key == GLFW_KEY_SLASH)							TextInput.push_back('/');
				}
				TextInputItem->SetLabel(TextInput);
				TextInputItem->Invalidate();
			}
			return;
		}

		if (inputMode == InputMode::Joystick)
		{
			Window->Scene->AppendConsole("KBM input mode activated.");
			inputMode = InputMode::KBM;
			moveType = MovementMode::Run;
		}

		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS && GLFW_MOD_ALT & mods)
		{
			currentMonitor--;
			if (currentMonitor < 0)
				currentMonitor = monitorCount - 1;
			RepositionWindow();
		}

		else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS && GLFW_MOD_ALT & mods)
		{
			currentMonitor++;
			if (currentMonitor == monitorCount)
				currentMonitor = 0;
			RepositionWindow();
		}

		else if (key == GLFW_KEY_C && action == GLFW_PRESS && GLFW_MOD_CONTROL & mods)
		{
			glfwSetWindowShouldClose(WindowPtr, 1);
		}

		else if (key == GLFW_KEY_S && action == GLFW_PRESS && GLFW_MOD_CONTROL & mods)
		{
			Settings::Audio::AudioEnabled = (Settings::Audio::AudioEnabled) ? 0 : 1;
			Window->Scene->AppendConsole((Settings::Audio::AudioEnabled) ? "Audio Enabled" : "Audio Disabled");
		}

		else if (key == GLFW_KEY_M && action == GLFW_PRESS && GLFW_MOD_CONTROL & mods)
		{
			Settings::Audio::MusicEnabled = (Settings::Audio::MusicEnabled) ? 0 : 1;
			Window->Scene->AppendConsole((Settings::Audio::MusicEnabled) ? "Music Enabled" : "Music Disabled");
		}

		else if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS)
		{
			if (!Console->IsMoving())
			{
				consoleShown = !consoleShown;
				if (consoleShown)
				{
					Settings::Video::ShowFPS = true;
					Settings::Misc::ShowConsole = true;
					Console->Move(glm::vec2(0, Console->GetDimensions().w + 10), 0.3f, Gui::Item::MoveStyle::Sinosoidal);
					FPSDisplay->Move(glm::vec2(0, -FPSDisplay->GetDimensions().w - 10), 0.3f, Gui::Item::MoveStyle::Sinosoidal);
				}
				else
				{
					Settings::Video::ShowFPS = false;
					Settings::Misc::ShowConsole = false;
					Console->Move(glm::vec2(0, -Console->GetDimensions().w - 10), 0.3f, Gui::Item::MoveStyle::Sinosoidal);
					FPSDisplay->Move(glm::vec2(0, FPSDisplay->GetDimensions().w + 10), 0.3f, Gui::Item::MoveStyle::Sinosoidal);
				}
			}
		}

		else if (key == GLFW_KEY_R && action == GLFW_PRESS)
		{
			moveType = (moveType == MovementMode::Run) ? MovementMode::Walk : MovementMode::Run;
		}

	}


	void Input::MouseButtonEvent(int button, int action, int mods)
	{
		if (inputMode == InputMode::TextInput)
		{
			inputMode = InputMode::KBM;
			TextInputItem->SaveValue();
			return;
		}

		if (inputMode == InputMode::Joystick)
		{
			Window->Scene->AppendConsole("KBM input mode activated.");
			inputMode = InputMode::KBM;
			moveType = MovementMode::Run;
		}

		if (action == GLFW_PRESS)
		{
			double x, y;
			glfwGetCursorPos(WindowPtr, &x, &y);
			MousePosition = glm::vec2(x, y);

			if (button == GLFW_MOUSE_BUTTON_1)	// Left Mouse Click
			{
				if (CameraMode::Game == cameraMode)
				{
					if (mods & GLFW_MOD_SHIFT)
					{
						// Primary Attack in Place
					}
					else
					{

					}
				}
			}

			else if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)	// Right Mouse Click
			{
			}
		}

		if (action == GLFW_PRESS)
		{
			Window->Scene->Gui->OnMouseDown(button);
		}
		else if (action == GLFW_RELEASE)
		{
			Window->Scene->Gui->OnMouseRelease(button);
		}

	}


	void Input::ScrollEvent(double x, double y)
	{
		if (cameraMode == CameraMode::Orbital)
		{
			orbitalCameraDistance -= (float)y * 0.25f;
			orbitalCameraDistance = glm::max(orbitalCameraDistance, 1.0f);
		}
	}


	void Input::RepositionWindow()
	{
		if (!Settings::Window::Fullscreen)
		{
			int xpos, ypos;
			glfwGetMonitorPos(monitors[currentMonitor], &xpos, &ypos);
			auto vm = glfwGetVideoMode(monitors[currentMonitor]);

			glfwSetWindowSize(WindowPtr, Settings::Window::DefaultWidth, Settings::Window::DefaultHeight);
			glfwSetWindowPos(WindowPtr, xpos + vm->width / 2 - Settings::Window::DefaultWidth / 2, ypos + vm->height / 2 - Settings::Window::DefaultHeight / 2);
		}
		else
		{
			int xpos, ypos;
			glfwGetMonitorPos(monitors[currentMonitor], &xpos, &ypos);
			auto vm = glfwGetVideoMode(monitors[currentMonitor]);

			glfwSetWindowSize(WindowPtr, vm->width, vm->height);
			glfwSetWindowPos(WindowPtr, xpos, ypos);
		}
	}


	void Input::SetTargetEntity(Entity* target)
	{
		Target = target;
	}


	void Input::SetCameraEntity(Entity* camera)
	{
		Camera = camera;
	}


	void Input::SetCharacterEntity(Entity* character)
	{
		Character = character;
	}


	Entity* Input::GetCharacterEntity()
	{
		return Character;
	}


	void Input::ToggleFullscreen()
	{
		Settings::Window::Fullscreen = !Settings::Window::Fullscreen;
		if (Settings::Misc::VerboseLogging)
			Debug::Log((Settings::Window::Fullscreen) ? "Switching to Fullscreen Mode" : "Switching to Windowed Mode");
		RepositionWindow();
	}


	void Input::SetCameraMode(CameraMode mode)
	{
		cameraMode = mode;
		Window->Scene->AppendConsole((cameraMode == CameraMode::Game) ? "Game Camera Mode" : "Orbital Camera Mode");
	}


	void Input::SetConsole(Gui::Item* console)
	{
		Console = console;
	}


	void Input::SetFPSDisplay(Gui::Item* item)
	{
		FPSDisplay = item;
	}


	void Input::ActivateTextInput(Gui::TextBox* textInputBox)
	{
		TextInputItem = textInputBox;
		OriginalTextInput = TextInputItem->GetLabel();
		TextInput = "";
		TextInputItem->SetLabel(TextInput);
		TextInputItem->Invalidate();
		inputMode = InputMode::TextInput;
	}


	glm::vec3 Input::CalculateMouseWorldSpaceVector()
	{
		if (Camera == nullptr)
			return glm::vec3();

		float viewPort[] = { 0.0f, 0.0f, (float)Settings::Window::Width, (float)Settings::Window::Height };
		glm::vec4 mouseVectorProjectionSpace(
			(CurrentMousePosition.x - viewPort[0]) / viewPort[2] * 2.0 - 1.0,
			-((CurrentMousePosition.y - viewPort[1]) / viewPort[3] * 2.0 - 1.0),
			2.0 * 1.0 - 1.0,
			1.0);
		glm::mat4 transformMatrix = glm::inverse(Camera->GetComponent<Core::Camera>()->GetProjectionMatrix() * Camera->GetComponent<Core::Camera>()->GetViewMatrix());
		glm::vec4 mouseVectorWorldSpace = transformMatrix * mouseVectorProjectionSpace;
		float inverseValue = 1.0f / mouseVectorWorldSpace.w;
		mouseVectorWorldSpace *= inverseValue;

		return glm::vec3(mouseVectorWorldSpace);
	}



	glm::vec2 Input::CalculateMousePosition()
	{
		auto p0 = Camera->Transform.Position;
		auto V = CalculateMouseWorldSpaceVector();
		auto N = glm::vec3(0, 1, 0);
		auto d = Character->Transform.Position.y;

		float t = -(glm::dot(p0, N) + d) / glm::dot(V, N);
		glm::vec3 p = p0 + t * V;
		return glm::vec2(p.x, p.z);
	}


	
	void Input::StopCharacter()
	{
		if (Character != nullptr)
		{
			auto c = Character->GetComponent<Core::Character>();
			if (c != nullptr)
				c->StopMoving();
		}
	}

}