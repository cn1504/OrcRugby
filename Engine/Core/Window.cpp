#include "Window.h"
#include "Assets.h"

namespace Core
{

	std::unordered_map<GLFWwindow*, Window*> WindowMap;


	Window::Window()
	{
		// Create a windowed mode window and its OpenGL context 
		glfwWindowHint(GLFW_DECORATED, GL_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		WindowPtr = glfwCreateWindow(
			Settings::Window::Width,
			Settings::Window::Height, 
			"Core", NULL, NULL);
		if (!WindowPtr)
			Debug::Error("Failed to creating main window.");
		WindowMap[WindowPtr] = this;

		glfwSetWindowPos(WindowPtr, Settings::Window::X, Settings::Window::Y);
		glfwSetWindowFocusCallback(WindowPtr, windowFocus_callback);
		glfwSetWindowIconifyCallback(WindowPtr, windowIconified_callback);
		glfwSetWindowSizeCallback(WindowPtr, windowResize_callback);
		glfwSetWindowPosCallback(WindowPtr, windowMove_callback);
		glfwSetKeyCallback(WindowPtr, windowKeyEvent_callback);
		glfwSetMouseButtonCallback(WindowPtr, windowMouseButtonEvent_callback);
		glfwSetScrollCallback(WindowPtr, windowScrollEvent_callback);

		// Make the window's opengl context current 
		glfwMakeContextCurrent(WindowPtr);
		if (Core::Settings::Video::VSync)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		HasFocus = true;

		// Initialize Glew
		if (GLEW_OK != glewInit()) {
			Debug::Error("Glew Failed to Initialize.");
		}
		glGetError();
						

		// Load Engine objects
		Input = new Core::Input(this, WindowPtr);
		Scene = new Core::Scene(this);

		LoopUntilClosed();
	}


	Window::~Window()
	{
		if (Settings::Misc::VerboseLogging)
			Debug::Log("Deleting Scene.");
		delete Scene;
		if (Settings::Misc::VerboseLogging)
			Debug::Log("Deleting Input.");
		delete Input;

		glfwDestroyWindow(WindowPtr);

		WindowMap.erase(WindowPtr);
	}


	void Window::LoopUntilClosed()
	{
		Assets::LoadStandardAssets();
		Scene->Load();

		while (!glfwWindowShouldClose(WindowPtr))
		{
			Time::NextUpdate(HasFocus);
			glfwPollEvents();
			Input->Update();
			Scene->Update();
			glfwSwapBuffers(WindowPtr);
		}
	}


	void Window::OnResize()
	{
		Scene->ResizeRenderBuffers();
		
		Scene->Gui->Resize(glm::vec2(Settings::Window::Width, Settings::Window::Height));
		Scene->Gui->Invalidate();
	}


	void Window::Close()
	{
		glfwSetWindowShouldClose(WindowPtr, 1);
	}


	void Window::Minimize()
	{
		glfwIconifyWindow(WindowPtr);
	}


	void Window::Resize(glm::vec2 delta)
	{
		glfwSetWindowSize(WindowPtr, Settings::Window::Width + (int)delta.x, Settings::Window::Height + (int)delta.y);
	}


	void Window::Move(glm::vec2 delta)
	{
		glfwSetWindowPos(WindowPtr, Settings::Window::X + (int)delta.x, Settings::Window::Y + (int)delta.y);

		Scene->Gui->Invalidate();
	}


	// Window Callbacks

	void windowIconified_callback(GLFWwindow* window, int state) 
	{
		WindowMap[window]->IsMinimized = (state == GL_TRUE);
	}
	
	void windowFocus_callback(GLFWwindow* window, int state)
	{
		WindowMap[window]->HasFocus = (state == GL_TRUE);
	}

	void windowMove_callback(GLFWwindow* window, int x, int y) 
	{
		Settings::Window::X = x;
		Settings::Window::Y = y;
	}

	void windowResize_callback(GLFWwindow* window, int width, int height) 
	{
		if (width < 1)
			return;

		Settings::Window::Width = width;
		Settings::Window::Height = height;

		WindowMap[window]->OnResize();
	}

	void windowKeyEvent_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		WindowMap[window]->Input->KeyEvent(key, scancode, action, mods);
	}

	void windowMouseButtonEvent_callback(GLFWwindow* window, int button, int action, int mods)
	{
		WindowMap[window]->Input->MouseButtonEvent(button, action, mods);
	}

	void windowScrollEvent_callback(GLFWwindow* window, double x, double y)
	{
		WindowMap[window]->Input->ScrollEvent(x, y);
	}
}