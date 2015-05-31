#pragma once
#include "MaterialViewer.h"
#include <Window/DefaultWindow.h>
#include <Window/WindowAction.h>
#include <Input/Action.h>
#include <Input/Map.h>

namespace MaterialViewer
{
	class MainWindow : public Core::Window::DefaultWindow
	{
	private:
		float elapsedTime;
		float cacheReloadTime;

		std::shared_ptr<Core::Input::Action> Reload;

	public:
		MainWindow(std::string material);
		virtual ~MainWindow();
	};


	class NewWindowAction : public Core::Input::Action
	{
	private:
		std::string Material;
	public:
		NewWindowAction(std::string material);
		virtual ~NewWindowAction();
		virtual void Perform();
	};

	class ReloadAssetsAction : public Core::Input::Action
	{
	public:
		ReloadAssetsAction();
		virtual ~ReloadAssetsAction();
		virtual void Perform();
	};

}