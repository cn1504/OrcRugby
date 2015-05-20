#include "AudioImporter.h"

#include "OpenFileDialog.h"
#include <Assets/Database.h>

std::shared_ptr<Core::Logger> AudioImporter::out;

using namespace AudioImporter;

void load(std::string filename)
{
	out->Log("Opening \"" + filename + "\"");

	auto first = filename.find_last_of("\\/") + 1;
	auto size = filename.find_last_of(".") - first;
	std::string name = filename.substr(first, size);

	// Load file
	SF_INFO sfinfo;
	sfinfo.format = 0;
	SNDFILE* soundfile = sf_open(filename.c_str(), SFM_READ, &sfinfo);

	auto framecount = sfinfo.frames * sfinfo.channels;
	short* dataBuffer = new short[framecount];
	auto readcount = sf_read_short(soundfile, dataBuffer, framecount);

	if (readcount != framecount)
	{
		Core::Debug->Error("Could not read all audio file frames");
	}

	// Read the sound format
	DWORD dataSize = (DWORD)(framecount * 2);  // 2 Bytes Per Frame
	DWORD sampleRate = sfinfo.samplerate;
	float duration = (float)sfinfo.frames / (float)sfinfo.samplerate;
	ALuint format = (sfinfo.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

	sf_close(soundfile);

	// Insert file into database
	auto b = new Core::Assets::Blob((unsigned char*)dataBuffer, dataSize);

	Core::Database->Sql("INSERT OR REPLACE INTO Audio (key, format, size, samplerate, duration, data) VALUES (?, ?, ?, ?, ?, ?)");
	Core::Database->Bind(1, name);
	Core::Database->Bind(2, static_cast<int>(format));
	Core::Database->Bind(3, static_cast<int>(dataSize));
	Core::Database->Bind(4, static_cast<int>(sampleRate));
	Core::Database->Bind(5, duration);
	Core::Database->Bind(6, *b);
	Core::Database->ExecuteAndFree();

	/*	Names
	std::ifstream in(filename);

	std::string Race = "Human";
	std::string Gender = "Female";
	
	std::string input;
	while (in >> input)
	{
		if (input.length() > 0)
		{
			Core::Database->Sql("INSERT OR REPLACE INTO Names (Name, Gender, Race) VALUES (?, ?, ?)");
			Core::Database->Bind(1, input);
			Core::Database->Bind(2, Gender);
			Core::Database->Bind(3, Race);
			Core::Database->ExecuteAndFree();
		}
		else
			break;
	}
	*/
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