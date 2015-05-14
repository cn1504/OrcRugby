#pragma once
#include <Core.h>
#include "Window.h"
#include <Renderers/GuiRenderer.h>
#include <Renderers/GeometryRenderer.h>
#include <Renderers/LightRenderer.h>
#include <Renderers/PostProcessingRenderer.h>
#include <Renderers/GBuffer.h>
#include <Renderers/LightBuffer.h>
#include "WindowAction.h"

namespace Core
{
	namespace Window
	{
		class DefaultWindow : public Window
		{
		protected:
			std::shared_ptr<Core::Components::Gui::Item> Top;
			std::shared_ptr<Core::Components::Gui::Item> Gui;
			std::shared_ptr<Core::Components::Camera> Camera;

			std::unique_ptr<Core::Renderers::GuiRenderer> GuiRenderer;
			std::unique_ptr<Core::Renderers::GeometryRenderer> GeometryRenderer;
			std::unique_ptr<Core::Renderers::LightRenderer> LightRenderer;
			std::unique_ptr<Core::Renderers::PostProcessingRenderer> PostProcessingRenderer;

			std::unique_ptr<Core::Renderers::GBuffer> GeometryBuffer;
			std::unique_ptr<Core::Renderers::LightBuffer> LightBuffer;

			std::shared_ptr<Core::Window::SwitchToFullscreen> FullscreenAction;
			std::shared_ptr<Core::Window::SwitchToWindowed> WindowedAction;
			std::shared_ptr<Core::Window::Iconify> IconifyAction;
			std::shared_ptr<Core::Window::MoveToPreviousMonitor> PreviousMonitorAction;
			std::shared_ptr<Core::Window::MoveToNextMonitor> NextMonitorAction;

		public:			
			DefaultWindow() = delete;
			DefaultWindow(const DefaultWindow&) = delete;
			DefaultWindow& operator=(const DefaultWindow&) = delete;
			DefaultWindow(DefaultWindow &&);
			DefaultWindow& operator=(DefaultWindow &&);

			DefaultWindow(std::string name, const glm::ivec2& position, const glm::ivec2& size);
			DefaultWindow(std::string name, const glm::ivec2& position, const glm::ivec2& size, std::unique_ptr<State> state);
			virtual ~DefaultWindow();
			
			virtual void Scale(const glm::ivec2& delta);
			virtual int Update();
		};



		class Minimized : public State
		{
		public:
			Minimized() = delete;
			Minimized(const Minimized&) = delete;
			Minimized& operator=(const Minimized&) = delete;
			Minimized(Minimized &&);
			Minimized& operator=(Minimized &&);
			Minimized(Window* context);
			virtual ~Minimized();
			virtual std::string GetName();
		};

		class Windowed : public State
		{
		public:
			Windowed() = delete;
			Windowed(const Windowed&) = delete;
			Windowed& operator=(const Windowed&) = delete;
			Windowed(Windowed &&);
			Windowed& operator=(Windowed &&);
			Windowed(Window* context);
			virtual ~Windowed();
			virtual std::string GetName();
		}; 
		
		class Fullscreen : public State
		{
		public:
			Fullscreen() = delete;
			Fullscreen(const Fullscreen&) = delete;
			Fullscreen& operator=(const Fullscreen&) = delete;
			Fullscreen(Fullscreen &&);
			Fullscreen& operator=(Fullscreen &&);
			Fullscreen(Window* context);
			virtual ~Fullscreen();
			virtual std::string GetName();
		};
	}
}