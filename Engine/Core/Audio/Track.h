#pragma once

#include <Core.h>
#include <Assets/AudioFile.h>

namespace Core
{
	namespace Audio
	{
		class Track
		{
		private:
			Core::Assets::AudioFile* file;
			Core::Components::Entity* bond;
			glm::vec3 position;
			glm::vec3 velocity;
			float volume;
			bool loop;

			float elapsedTime;

			ALuint buffer;
			ALuint source;

			void Play();

		public:
			Track(Core::Assets::AudioFile* file, Core::Components::Entity* bond, float volume, bool loop);
			Track(Core::Assets::AudioFile* file, glm::vec3 position, glm::vec3 velocity, float volume, bool loop);
			~Track();

			void Update();
			bool HasFinished();
		};
	}
}