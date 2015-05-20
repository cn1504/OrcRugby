#pragma once
#include <Game.h>
#include <Window/DefaultWindow.h>
#include <Window/WindowAction.h>
#include <Input/Action.h>
#include <Input/Map.h>
#include <Components/Gui/Item.h>
#include <Components/Gui/Button.h>
#include <Components/CameraControls.h>
#include <Components/SaveDB.h>
#include <Components/Generator.h>

namespace Game
{
	class ClickAction;
	class ClickDownAction;

	class MainWindow : public Core::Window::DefaultWindow
	{
		friend class ClickAction;
		friend class ClickDownAction;

	protected:
		std::vector<std::shared_ptr<Core::Components::Gui::Item>> Layers;
		std::shared_ptr<Core::Components::Gui::Item> FPSLabel;

		std::shared_ptr<EntityMovementAction> CameraUp;
		std::shared_ptr<EntityMovementAction> CameraDown;
		std::shared_ptr<EntityMovementAction> CameraLeft;
		std::shared_ptr<EntityMovementAction> CameraRight;
		std::shared_ptr<EntityMovementAction> CameraIn;
		std::shared_ptr<EntityMovementAction> CameraOut;
		std::shared_ptr<CameraRotationAction> CameraRotateLeft;
		std::shared_ptr<CameraRotationAction> CameraRotateRight;

		std::shared_ptr<Game::States::GameStateController> GSC;
		std::shared_ptr<Core::Input::Action> OnMouseLeftRelease;
		std::shared_ptr<Core::Input::Action> OnMouseLeftDown;
		std::vector<std::weak_ptr<Core::Components::Gui::Button>> Buttons;
		bool UpdateButtons(std::shared_ptr<Core::Space::Transform2DIF> parent, const glm::vec2& mp, bool deactivate);
				
		std::unique_ptr<Game::Components::Generator> Generator;

	public:
		MainWindow();
		virtual ~MainWindow();
		virtual int Update();
		
		virtual void Translate(const glm::ivec2& delta);
		virtual void Scale(const glm::ivec2& delta);

		void AddGuiItemToLayer(int i, std::shared_ptr<Core::Components::Gui::Item> item);
		void AddButton(std::shared_ptr<Core::Components::Gui::Button> button);
	};


	class NewGameWindowAction : public Core::Input::Action
	{
	public:
		virtual ~NewGameWindowAction();
		virtual void Perform();
	};



	class ClickAction : public Core::Input::Action
	{
	private:
		MainWindow* Window;

	public:
		ClickAction(MainWindow* Window) : Window(Window) {}
		
		virtual void Perform();
	};

	class ClickDownAction : public Core::Input::Action
	{
	private:
		MainWindow* Window;

	public:
		ClickDownAction(MainWindow* Window) : Window(Window) {}

		virtual void Perform();
	};
	
}