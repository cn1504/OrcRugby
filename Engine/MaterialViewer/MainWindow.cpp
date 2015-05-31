#include "MainWindow.h"
#include <Assets/AssetDB.h>
#include <Components/StaticMesh.h>
#include <Components/LightSource.h>
#include "LightMotionControl.h"

using namespace MaterialViewer;

MainWindow::MainWindow(std::string material)
	: DefaultWindow("Material Viewer",
	glm::ivec2((1920 - 768), (1080 - 768) / 2),
		glm::ivec2(768, 768))
{
	Camera->Translate(glm::vec3(0.0, 0.0, -1.4));
	//Camera->Rotate(glm::vec3(0.0, glm::radians(-45.0), 0.0));
	
	if (Core::Window::Map.size() < 2)
	{
		// Add objects to the scene.
		Core::Debug->Log("Viewing " + material);
		auto cube = std::make_shared<Core::Components::StaticMesh>("Billboard", material);
		cube->Translate(glm::vec3(0.0, 0.0, -0.5));
		Core::Scene->AddChild(cube);

		auto light = std::make_shared<Core::Components::LightSource>("Test");
		light->Scale(glm::vec3(10.0, 10.0, 10.0));
		auto mc = std::make_shared<LightMotionControl>();
		mc->AddChild(light);
		Core::Scene->AddChild(mc);
	}
	
	Reload = std::make_shared<ReloadAssetsAction>();
	InputMap->AddReleaseAction("Space", Reload);
}
MainWindow::~MainWindow() {}


NewWindowAction::NewWindowAction(std::string material) : Material(material) {}
NewWindowAction::~NewWindowAction() {}
void NewWindowAction::Perform()
{
	new MainWindow(Material);
}


ReloadAssetsAction::ReloadAssetsAction() {}
ReloadAssetsAction::~ReloadAssetsAction() {}
void ReloadAssetsAction::Perform()
{
	Core::AssetDB->ClearAllCaches();
}