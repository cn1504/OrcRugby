#pragma once
#include "Core.h"

namespace Core
{
	
	class Entity;
	class AudioFile;


	class Track
	{
	private:
		AudioFile* file;
		Entity* bond;
		glm::vec3 position;
		glm::vec3 velocity;
		float volume;
		bool loop;

		float elapsedTime;

		ALuint buffer;
		ALuint source;

		void Play();

	public:
		Track(AudioFile* file, Entity* bond, float volume, bool loop);
		Track(AudioFile* file, glm::vec3 position, glm::vec3 velocity, float volume, bool loop);
		~Track();

		void Update();
		bool HasFinished();
	};


	class AL
	{
	private:
		ALCdevice *device;
		ALCcontext *ctx;
		Entity *entity;
		glm::vec3 lastPosition;
		std::vector<Track*> tracklist;

	public:
		void Init();
		void Update();
		void Close();

		void SetCurrentListener(Entity* e);
		void PlayTrack(Track* track);

	};

	extern AL AudioListener;

}