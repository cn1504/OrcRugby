#include "AudioFile.h"

using namespace Core::Assets;


AudioFile::AudioFile(void* dataBuffer, size_t bufferSize, ALuint format, Core::Assets::DWORD dataSize, Core::Assets::DWORD sampleRate, float duration, Type type, bool loop)
	: format(format), dataSize(dataSize), sampleRate(sampleRate), duration(duration), type(type), loop(loop)
{	
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, dataBuffer, dataSize, sampleRate);
}


AudioFile::~AudioFile()
{
	alDeleteBuffers(1, &buffer);
}