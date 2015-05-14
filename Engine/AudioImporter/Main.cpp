#include "AudioImporter.h"

#include "OpenFileDialog.h"
#include <Assets/Database.h>

std::shared_ptr<Core::Logger> AudioImporter::out;

using namespace AudioImporter;

void load(std::string filename)
{
	out->Log("Opening \"" + filename + "\"");

	/*
	unsigned int width;
	unsigned int height;
	std::vector<unsigned char> image;

	auto r = LibTexture::LoadPNG(image, width, height, filename);

	auto first = filename.find_last_of("\\/") + 1;
	auto size = filename.find_last_of(".") - first;
	std::string name = filename.substr(first, size);
	auto b = std::make_unique<Core::Assets::Blob>(image);
	out->Log("Name: " + name);
	out->Log("Resolution: " + std::to_string(width) + "x" + std::to_string(height));

	Core::Database->Sql("INSERT OR REPLACE INTO Textures (key, width, height, data) VALUES (?, ?, ?, ?)");
	Core::Database->Bind(1, name);
	Core::Database->Bind(2, static_cast<int>(width));
	Core::Database->Bind(3, static_cast<int>(height));
	Core::Database->Bind(4, *b);
	Core::Database->ExecuteAndFree();
	*/
}

int main(int argc, char* argv[])
{
	try {
		Core::Init(argc, argv);

		out = std::make_shared<Core::Logger>("AudioImporter.txt");

		if (argc > 1)
		{
			for (int i = 1; i < argc; i++)
			{
				load(argv[i]);
			}
		}
		else
		{
			auto ofd = std::make_unique<OpenFileDialog>();
			ofd->Title = _T("Choose Audio File To Import");
			if (ofd->ShowDialog())
			{
				load(ofd->FileName);
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