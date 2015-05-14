#include "MeshImporter.h"

#include "Importer.h"
#include "OpenFileDialog.h"

std::shared_ptr<Core::Logger> MeshImporter::out;

using namespace MeshImporter;

int main(int argc, char* argv[])
{
	try {
		Core::Init(argc, argv);
		out = std::make_shared<Core::Logger>("MeshImporter.txt");
		
		if (argc > 1)
		{
			for (int i = 1; i < argc; i++)
			{
				out->Log("Opening \"" + std::string(argv[i]) + "\"");

				LoadFile(argv[i]);
			}
		}
		else
		{
			auto ofd = std::make_unique<OpenFileDialog>();
			ofd->Title = _T("Choose Mesh File To Import");
			if (ofd->ShowDialog())
			{
				out->Log("Opening \"" + std::string(ofd->FileName) + "\"");

				LoadFile(ofd->FileName);
			}
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