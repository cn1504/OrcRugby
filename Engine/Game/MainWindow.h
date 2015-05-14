#pragma once
#include <Game.h>
#include <Window/DefaultWindow.h>
#include <Window/WindowAction.h>
#include <Input/Action.h>
#include <Components/Gui/Item.h>
#include <Components/CameraControls.h>
#include <Components/SaveDB.h>

namespace Game
{
	class MainWindow : public Core::Window::DefaultWindow
	{
	protected:
		std::shared_ptr<Core::Components::Gui::Item> FPSLabel;

		std::shared_ptr<EntityMovementAction> CameraUp;
		std::shared_ptr<EntityMovementAction> CameraDown;
		std::shared_ptr<EntityMovementAction> CameraLeft;
		std::shared_ptr<EntityMovementAction> CameraRight;
		std::shared_ptr<EntityMovementAction> CameraIn;
		std::shared_ptr<EntityMovementAction> CameraOut;
		std::shared_ptr<CameraRotationAction> CameraRotateLeft;
		std::shared_ptr<CameraRotationAction> CameraRotateRight;

		std::shared_ptr<Core::Components::Gui::Item> MMBackground;
		std::shared_ptr<Game::Components::GameStateController> GSC;

				
	public:
		MainWindow();
		virtual ~MainWindow();
		virtual int Update();
		
		virtual void Translate(const glm::ivec2& delta);
		virtual void Scale(const glm::ivec2& delta);
	};


	class NewGameWindowAction : public Core::Input::Action
	{
	public:
		virtual ~NewGameWindowAction();
		virtual void Perform();
	};
	
}