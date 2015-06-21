#include "Window.h"
#include <Input/Input.h>

GLEWContext* glewGetContext()
{
	return Core::Window::CurrentContext->GetGLEWContext();
}


static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
	std::stringstream stringStream;
	std::string sourceString;
	std::string typeString;
	std::string severityString;

	// The AMD variant of this extension provides a less detailed classification of the error,
	// which is why some arguments might be "Unknown".
	switch (source) {
	case GL_DEBUG_CATEGORY_API_ERROR_AMD:
	case GL_DEBUG_SOURCE_API: {
		sourceString = "API";
		break;
	}
	case GL_DEBUG_CATEGORY_APPLICATION_AMD:
	case GL_DEBUG_SOURCE_APPLICATION: {
		sourceString = "Application";
		break;
	}
	case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD:
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
		sourceString = "Window System";
		break;
	}
	case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:
	case GL_DEBUG_SOURCE_SHADER_COMPILER: {
		sourceString = "Shader Compiler";
		break;
	}
	case GL_DEBUG_SOURCE_THIRD_PARTY: {
		sourceString = "Third Party";
		break;
	}
	case GL_DEBUG_CATEGORY_OTHER_AMD:
	case GL_DEBUG_SOURCE_OTHER: {
		sourceString = "Other";
		break;
	}
	default: {
		sourceString = "Unknown";
		break;
	}
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR: {
		typeString = "Error";
		break;
	}
	case GL_DEBUG_CATEGORY_DEPRECATION_AMD:
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
		typeString = "Deprecated Behavior";
		break;
	}
	case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD:
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
		typeString = "Undefined Behavior";
		break;
	}
	case GL_DEBUG_TYPE_PORTABILITY_ARB: {
		typeString = "Portability";
		break;
	}
	case GL_DEBUG_CATEGORY_PERFORMANCE_AMD:
	case GL_DEBUG_TYPE_PERFORMANCE: {
		typeString = "Performance";
		break;
	}
	case GL_DEBUG_CATEGORY_OTHER_AMD:
	case GL_DEBUG_TYPE_OTHER: {
		typeString = "Other";
		break;
	}
	default: {
		typeString = "Unknown";
		break;
	}
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: {
		severityString = "High";
		break;
	}
	case GL_DEBUG_SEVERITY_MEDIUM: {
		severityString = "Medium";
		break;
	}
	case GL_DEBUG_SEVERITY_LOW: {
		severityString = "Low";
		break;
	}
	default: {
		severityString = "Unknown";
		break;
	}
	}

	stringStream << "OpenGL Error: " << msg;
	stringStream << " [Source = " << sourceString;
	stringStream << ", Type = " << typeString;
	stringStream << ", Severity = " << severityString;
	stringStream << ", ID = " << id << "]";

	return stringStream.str();
}

static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) {
	std::string error = FormatDebugOutput(source, type, id, severity, message);
	Core::Debug->Log(error);
}


namespace Core
{
	namespace Window
	{

		std::unordered_map<GLFWwindow*, Window*> Map;
		Window* CurrentContext = nullptr;



		// Window Callbacks
		void windowPos_callback(GLFWwindow* window, int xpos, int ypos)
		{
			Map[window]->Translate(glm::ivec2(xpos, ypos) - Map[window]->GetPosition());
		}


		void windowSize_callback(GLFWwindow* window, int width, int height)
		{
			Map[window]->Scale(glm::ivec2(width, height) - Map[window]->GetSize());			
		}
		

		void windowIconified_callback(GLFWwindow* window, int state)
		{
//			if (state == GL_TRUE)
//				Map[window]->Minimize();
//			else
//				Map[window]->Restore();
		}

		void windowFocus_callback(GLFWwindow* window, int state)
		{
//			if (state == GL_TRUE)
//				Map[window]->Focus();
//			else
//				Map[window]->Defocus();
		}


		// Input Callbacks

		void windowKeyEvent_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Core::Input::GlobalMap.KeyEvent(key, scancode, action, mods);
			Map[window]->InputMap->KeyEvent(key, scancode, action, mods);
		}

		void windowMouseButtonEvent_callback(GLFWwindow* window, int button, int action, int mods)
		{
			Core::Input::GlobalMap.MouseButtonEvent(button, action, mods);
			Map[window]->InputMap->MouseButtonEvent(button, action, mods);
		}

		void windowScrollEvent_callback(GLFWwindow* window, double x, double y)
		{
			Core::Input::GlobalMap.ScrollEvent(x, y);
			Map[window]->InputMap->ScrollEvent(x, y);
		}



		Window::Window(Window && o)
		{
			GLFWWindowPtr = o.GLFWWindowPtr;
			GLEWContextPtr = o.GLEWContextPtr;

			Title = std::move(o.Title);
			Position = o.Position;
			Size = o.Size;

			CurrentState = std::move(o.CurrentState);
			InputMap = std::move(o.InputMap); 
			ScreenBuffer = std::move(o.ScreenBuffer);
		}

		Window& Window::operator=(Window && o)
		{
			GLFWWindowPtr = o.GLFWWindowPtr;
			GLEWContextPtr = o.GLEWContextPtr;

			Title = std::move(o.Title);
			Position = o.Position;
			Size = o.Size;

			CurrentState = std::move(o.CurrentState);
			InputMap = std::move(o.InputMap);
			ScreenBuffer = std::move(o.ScreenBuffer);
		}

		Window::Window(std::string title, const glm::ivec2& position, const glm::ivec2& size, std::unique_ptr<State> initialState)
		{
			Title = std::move(title);
			Position = position;
			Size = size;
			CurrentState = std::move(initialState);
			InputMap = std::make_unique<Core::Input::Map>();

			// Create a windowed mode window and its OpenGL context
			glfwWindowHint(GLFW_DECORATED, GL_FALSE);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
			//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
			GLFWWindowPtr = glfwCreateWindow(
				Size.x, Size.y,	Title.c_str(), NULL, (CurrentContext) ? CurrentContext->GetGLFWWindow() : NULL);
			if (!GLFWWindowPtr)
				Debug->Error("Failed to creating main window.");
			Map[GLFWWindowPtr] = this;
			glfwSetWindowPos(GLFWWindowPtr, Position.x, Position.y);

			glfwSetWindowPosCallback(GLFWWindowPtr, windowPos_callback);
			glfwSetWindowSizeCallback(GLFWWindowPtr, windowSize_callback);
			glfwSetWindowFocusCallback(GLFWWindowPtr, windowFocus_callback);
			glfwSetWindowIconifyCallback(GLFWWindowPtr, windowIconified_callback);
			glfwSetKeyCallback(GLFWWindowPtr, windowKeyEvent_callback);
			glfwSetMouseButtonCallback(GLFWWindowPtr, windowMouseButtonEvent_callback);
			glfwSetScrollCallback(GLFWWindowPtr, windowScrollEvent_callback);
			
			// Make the window's opengl context current
			glfwMakeContextCurrent(GLFWWindowPtr);
			glfwSwapInterval(1);

			// Initialize Glew
			GLEWContextPtr = new GLEWContext();
			BeginUpdate();
			if (GLEW_OK != glewInit()) {
				Debug->Error("Glew Failed to Initialize.");
			}
			glGetError();

			// OpenGL Debugging
			// SUPER VERBOSE DEBUGGING!
			if (glDebugMessageControl != NULL) {
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
				glDebugMessageCallback((GLDEBUGPROCARB)DebugCallback, NULL);
			}

			ScreenBuffer = std::make_unique<Core::Renderers::ScreenBuffer>(size);
		}

		Window::~Window() {
			if (Map.size() == 1)
			{
				Scene = nullptr;
				AssetDB = nullptr;
			}
			glfwDestroyWindow(GLFWWindowPtr);
		}

		GLFWwindow* Window::GetGLFWWindow() { return GLFWWindowPtr; }
		GLEWContext* Window::GetGLEWContext() { return GLEWContextPtr; }

		std::string Window::GetTitle() { return Title; }
		glm::ivec2 Window::GetPosition() { return Position; }
		glm::ivec2 Window::GetSize() { return Size; }

		void Window::Translate(const glm::ivec2& delta) { Position += delta; }
		void Window::Scale(const glm::ivec2& delta)
		{
			Size += delta;

			ScreenBuffer->Scale(delta);
		}

		int Window::Update()
		{
			BeginUpdate();

			if (glfwWindowShouldClose(GLFWWindowPtr))
			{
				return 1;
			}

			glfwSwapBuffers(GLFWWindowPtr);
			return 0;
		}

		void Window::BeginUpdate()
		{
			glfwMakeContextCurrent(GLFWWindowPtr);
			CurrentContext = this;

			InputMap->Update();
		}

		// State Based Actions
		void Window::Close() { CurrentState->Close(); }
		void Window::Minimize() { CurrentState->Minimize(); }
		void Window::Maximize() { CurrentState->Maximize(); }
		void Window::Restore() { CurrentState->Restore(); }
		void Window::Move(const glm::ivec2& delta) { CurrentState->Move(delta); }
		void Window::Resize(const glm::ivec2& delta) { CurrentState->Resize(delta); }
		void Window::Focus() { CurrentState->Focus(); }
		void Window::Defocus() { CurrentState->Defocus(); }
		
		void Window::SetState(std::unique_ptr<State> state)
		{
			CurrentState = std::move(state);
		}
	}
}