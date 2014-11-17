#pragma once
#include "Core.h"
#include "Scene.h"
#include "Input.h"

namespace Core
{

	class Window
	{
	private:
		GLFWwindow* WindowPtr;

	public:
		Input* Input;
		Scene* Scene;

		bool HasFocus;
		bool IsMinimized;

		Window();
		~Window();

		void LoopUntilClosed();
		void OnResize();
		void Close();
		void Minimize();
		void Resize(glm::vec2 delta);
		void Move(glm::vec2 delta);
	};


	// GLFW Window Callbacks
	extern std::unordered_map<GLFWwindow*, Window*> WindowMap;
	void windowIconified_callback(GLFWwindow* window, int state);
	void windowFocus_callback(GLFWwindow* window, int state);
	void windowMove_callback(GLFWwindow* window, int x, int y);
	void windowResize_callback(GLFWwindow* window, int width, int height);
	void windowKeyEvent_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void windowMouseButtonEvent_callback(GLFWwindow* window, int button, int action, int mods);
	void windowScrollEvent_callback(GLFWwindow* window, double x, double y);

}