#include "AudioFile.h"

using namespace Core::Assets;


AudioFile::AudioFile(std::string filename, Type t)
{
	dataBuffer = NULL;

	/*
	SF_INFO sfinfo;
	sfinfo.format = 0;
	SNDFILE* soundfile = sf_open(("Resources/Audio/" + filename).c_str(), SFM_READ, &sfinfo);

	auto framecount = sfinfo.frames * sfinfo.channels;
	dataBuffer = new short[framecount];
	auto readcount = sf_read_short(soundfile, dataBuffer, framecount);

	if (readcount != framecount)
	{
		Debug::Error("Could not read all audio file frames");
	}

	// Tell OpenAL the sound format
	dataSize = (DWORD)(framecount * 2);  // 2 Bytes Per Frame
	sampleRate = sfinfo.samplerate;
	duration = (float)sfinfo.frames / (float)sfinfo.samplerate;
	format = (sfinfo.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

	sf_close(soundfile);
	*/

	type = t;
}


AudioFile::~AudioFile()
{
	delete[] dataBuffer;
}