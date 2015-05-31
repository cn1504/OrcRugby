#include "MaterialViewer.h"
#include "MainWindow.h"
#include <Input/Input.h>
#include <Assets/Database.h>

using namespace MaterialViewer;

int main(int argc, char* argv[])
{
	try {
		Core::Init(argc, argv);
				
		if (argc > 1)
		{
			// Open the main window
			auto createWindowAction = std::make_shared<NewWindowAction>(argv[1]);
			Core::Input::GlobalMap.AddReleaseAction("Ctrl + Alt + N", std::weak_ptr<Core::Input::Action>(createWindowAction));
			createWindowAction->Perform();

			Core::Run();
		}
		else
		{			
			Core::Debug->Log("Invalid Material.");
		}

		Core::Close();
	}
	catch (std::runtime_error &e) {
		Core::Debug->Log(e.what());
		Core::Close();
		exit(EXIT_FAILURE);
	}
	return 0;
}