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
			std::string file;
			Core::Components::Entity* bond;
			glm::vec3 position;
			glm::vec3 velocity;
			float volume;
			bool loop;

			float elapsedTime;
			float duration;

			ALuint source;

			void Play();

		public:
			Track(std::string file, Core::Components::Entity* bond, float volume, bool loop);
			Track(std::string file, glm::vec3 position, glm::vec3 velocity, float volume, bool loop);
			~Track();

			void Update();
			bool HasFinished();
		};
	}
}