#pragma once
#include "Core.h"

namespace Core
{

	typedef unsigned long DWORD;
	typedef unsigned char BYTE;

	class AudioFile
	{
	public:
		enum class Type { SOUND_EFFECT, AMBIENT, VOICE, MUSIC };

		ALuint format;

		short* dataBuffer;
		DWORD dataSize;
		DWORD sampleRate;
		float duration;
		Type type;

		AudioFile(std::string filename, Type t = Type::SOUND_EFFECT);
		~AudioFile();
	};

}