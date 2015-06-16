#pragma once

#include <Core.h>
#include "Entity.h"

namespace Core
{
	namespace Components
	{
		class AudioSource;

		class AudioSourceUpdateBehavior
		{
		protected:
			AudioSource* AS;
		public:
			AudioSourceUpdateBehavior(AudioSource* AS) : AS(AS) {}
			virtual ~AudioSourceUpdateBehavior() {}
			virtual void Update() {}
		};

		class AudioSourceFadeToNew : public AudioSourceUpdateBehavior
		{
		protected:
			std::string NextFile;
			float Elapsed;
			float Duration;
			float InitialVolume;
		public:
			AudioSourceFadeToNew(AudioSource* as, std::string nextFile, float duration);
			virtual ~AudioSourceFadeToNew() {}
			virtual void Update();
		};



		class AudioSource : public Entity
		{
			friend class AudioSourceFadeToNew;

		private:
			std::string File;

			ALuint Source;
			float ElapsedTime;
			float Duration;
			float Volume;

			glm::vec3 Position;

			std::unique_ptr<AudioSourceUpdateBehavior> UpdateBehavior;
						
		public:
			AudioSource(std::string audioFile);
			virtual ~AudioSource();

			virtual void Update();

			void Play();
			void Stop();
			void FadeToNewFile(std::string file, float duration);
		};
	}
}