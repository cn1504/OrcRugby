#pragma once
#include <Core.h>
#include <Space/Translatable.h>
#include <Space/Scalable.h>
#include <Renderers/ScreenBuffer.h>
#include "State.h"

namespace Core
{
	namespace Window
	{
		
		// GLFW Window Callbacks
		extern std::unordered_map<GLFWwindow*, Window*> Map;
		extern Window* CurrentContext;
		

		class Window : 
			public Core::Space::Translatable2DInt, 
			public Core::Space::Scalable2DInt
		{
		private:
			GLFWwindow* GLFWWindowPtr;
			GLEWContext* GLEWContextPtr;
			
		protected:
			std::string Title;
			glm::ivec2 Position;
			glm::ivec2 Size;

			std::unique_ptr<State> CurrentState;
			std::unique_ptr<Core::Renderers::ScreenBuffer> ScreenBuffer;

			void BeginUpdate();

		public:
			std::unique_ptr<Core::Input::Map> InputMap;

			Window() = delete;
			Window(const Window&) = delete;
			Window& operator=(const Window&) = delete;
			Window(Window &&);
			Window& operator=(Window &&);
			Window(std::string title, const glm::ivec2& position, const glm::ivec2& size, std::unique_ptr<State> initialState);
			virtual ~Window();

			GLFWwindow* GetGLFWWindow();
			GLEWContext* GetGLEWContext();

			std::string GetTitle();
			glm::ivec2 GetPosition();
			glm::ivec2 GetSize();

			virtual void Translate(const glm::ivec2& delta);
			virtual void Scale(const glm::ivec2& delta);

			virtual int Update();

			void SetState(std::unique_ptr<State> state);

			// State Based Actions
			void Close();
			void Minimize();
			void Maximize();
			void Restore();
			void Move(const glm::ivec2& delta);
			void Resize(const glm::ivec2& delta);
			void Focus();
			void Defocus();
		};
	}

}