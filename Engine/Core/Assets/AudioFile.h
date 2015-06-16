#pragma once
#include <Core.h>

namespace Core
{
	namespace Assets
	{

		typedef unsigned long DWORD;
		typedef unsigned char BYTE;

		class AudioFile
		{
		public:
			enum class Type { SOUND_EFFECT, AMBIENT, VOICE, MUSIC };

			ALuint buffer;

			ALuint format;
			DWORD dataSize;
			DWORD sampleRate;
			float duration;
			Type type; 
			bool loop;

			AudioFile(void* dataBuffer, size_t bufferSize, ALuint format, DWORD dataSize, DWORD sampleRate, float duration, Type type, bool loop);
			~AudioFile();
		};

	}
}