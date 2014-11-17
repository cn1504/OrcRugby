#include "AudioListener.h"
#include "Entity.h"
#include "AudioFile.h"
#include "RigidBody.h"

namespace Core
{

	AL AudioListener;


	void AL::Init()
	{
		device = NULL;
		ctx = NULL;

		/* Open and initialize a device with default settings */
		device = alcOpenDevice("OpenAL Soft\0");
		if (!device)
		{
			Settings::Audio::AudioEnabled = 0;
			return;
		}

		ctx = alcCreateContext(device, NULL);
		if (ctx == NULL || alcMakeContextCurrent(ctx) == ALC_FALSE)
		{
			if (ctx != NULL)
				alcDestroyContext(ctx);
			alcCloseDevice(device);

			Settings::Audio::AudioEnabled = 0;
			return;
		}

		if (Settings::Misc::VerboseLogging)
		{
			Debug::Log("Audio Device: " + std::string(alcGetString(device, ALC_DEVICE_SPECIFIER)) + 
				", " + std::to_string(alIsExtensionPresent("AL_EXT_MCFORMATS")) + "\n");
		}
	}


	void AL::Close(void)
	{
		for (auto t : tracklist)
		{
			delete t;
		}
		tracklist.clear();

		ALCdevice *device;
		ALCcontext *ctx;

		ctx = alcGetCurrentContext();
		if (ctx == NULL)
			return;

		device = alcGetContextsDevice(ctx);

		alcMakeContextCurrent(NULL);
		alcDestroyContext(ctx);
		alcCloseDevice(device);
	}


	void AL::SetCurrentListener(Entity* e)
	{
		entity = e;
		lastPosition = entity->Transform.Position;
	}


	void AL::Update()
	{
		if (Settings::Audio::AudioEnabled)
		{
			// Update Listener
			glm::vec3 forward = entity->Transform.WSForward();
			glm::vec3 up = entity->Transform.WSUp();
			glm::vec3 vel = (entity->Transform.Position - lastPosition) / Time::Delta;
			ALfloat ListenerPos[] = { entity->Transform.Position.x, entity->Transform.Position.y, entity->Transform.Position.z };
			ALfloat ListenerVel[] = { vel.x, vel.y, vel.z };
			ALfloat ListenerOri[] = {
				forward.x, forward.y, forward.z,	// Facing Direction
				up.x, up.y, up.z					// Up Direction
			};
			alListenerfv(AL_POSITION, ListenerPos);
			alListenerfv(AL_VELOCITY, ListenerVel);
			alListenerfv(AL_ORIENTATION, ListenerOri);

			alListenerf(AL_GAIN, Settings::Audio::MainVolume);

			lastPosition = entity->Transform.Position;
		}

		for (auto t : tracklist)
		{
			t->Update();
		}

		for (int i = 0; i < tracklist.size(); i++)
		{
			if (tracklist[i]->HasFinished())
			{
				delete tracklist[i];
				tracklist[i] = tracklist[tracklist.size() - 1];
				tracklist.resize(tracklist.size() - 1);
				i--;
			}
		}
	}


	void AL::PlayTrack(Track* track)
	{
		tracklist.push_back(track);
	}


	Track::Track(AudioFile* file, Entity* bond, float volume, bool loop)
	{
		this->file = file;
		this->bond = bond;
		this->volume = volume;
		this->loop = loop;

		// Load file
		alGenBuffers(1, &buffer);
		alGenSources(1, &source);

		alBufferData(buffer, file->format, file->dataBuffer, file->dataSize, file->sampleRate);

		elapsedTime = 0.0f;

		Play();
	}


	Track::Track(AudioFile* file, glm::vec3 position, glm::vec3 velocity, float volume, bool loop)
	{
		this->file = file;
		this->bond = nullptr;
		this->volume = volume;
		this->loop = loop;

		alGenBuffers(1, &buffer);
		alGenSources(1, &source);

		alBufferData(buffer, file->format, file->dataBuffer, file->dataSize, file->sampleRate);

		elapsedTime = 0.0f;
		this->position = position;
		this->velocity = velocity;

		Play();
	}


	Track::~Track()
	{
		// Finish
		alSourceStop(source);

		alDeleteSources(1, &source);
		alDeleteBuffers(1, &buffer);
	}


	void Track::Update()
	{
		float volume = 1.0f;
		if (file->type == AudioFile::Type::SOUND_EFFECT)
			volume = (Settings::Audio::AudioEnabled) ? Settings::Audio::SoundEffectsVolume : 0.0f;
		else if (file->type == AudioFile::Type::AMBIENT)
			volume = (Settings::Audio::AudioEnabled) ? Settings::Audio::AmbientEffectsVolume : 0.0f;
		else if (file->type == AudioFile::Type::VOICE)
			volume = (Settings::Audio::AudioEnabled) ? Settings::Audio::VoiceVolume : 0.0f;
		else if (file->type == AudioFile::Type::MUSIC)
			volume = (Settings::Audio::MusicEnabled && Settings::Audio::AudioEnabled) ? Settings::Audio::MusicVolume : 0.0f;

		alSourcef(source, AL_GAIN, this->volume * volume);

		if (bond != nullptr)
		{
			position = bond->Transform.Position;
			velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		alSourcefv(source, AL_POSITION, glm::value_ptr(position));
		alSourcefv(source, AL_VELOCITY, glm::value_ptr(velocity));

		elapsedTime += Time::RealtimeDelta;
	}


	void Track::Play()
	{
		Update();

		alSourcei(source, AL_BUFFER, buffer);
		alSourcef(source, AL_PITCH, 1.0f);
		alSourcei(source, AL_LOOPING, loop);

		alSourcePlay(source);
	}


	bool Track::HasFinished()
	{
		return elapsedTime > file->duration;
	}

}