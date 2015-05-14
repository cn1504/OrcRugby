#include <Editor.h>
#include "MainWindow.h"
#include <Input/Input.h>
#include <Assets/Database.h>

int main(int argc, char* argv[])
{
	try {
		Core::Init(argc, argv);

		// Initialize scene
		

		// Open the main window
		auto createWindowAction = std::make_shared<Editor::NewEditorWindowAction>();
		Core::Input::GlobalMap.AddReleaseAction("Ctrl + Alt + N", std::weak_ptr<Core::Input::Action>(createWindowAction));
		createWindowAction->Perform();
		
		Core::Run();	
	}
	catch (std::runtime_error &e) {
		Core::Debug->Log(e.what());
		Core::Close();
		exit(EXIT_FAILURE);
	}
	return 0;
}