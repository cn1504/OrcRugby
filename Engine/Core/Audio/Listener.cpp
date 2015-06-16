#include "Listener.h"

using namespace Core::Audio;

void AL::Init()
{
	AudioEnabled = false;
	MusicEnabled = true;
	Volume = 1.0f;
	MusicVolume = 1.0f;
	AmbientVolume = 1.0f;
	SFXVolume = 1.0f;
	VoiceVolume = 1.0f;

	device = NULL;
	ctx = NULL;

	/* Open and initialize a device with default settings */
	device = alcOpenDevice("OpenAL Soft\0");
	if (!device)
	{
		return;
	}

	ctx = alcCreateContext(device, NULL);
	if (ctx == NULL || alcMakeContextCurrent(ctx) == ALC_FALSE)
	{
		if (ctx != NULL)
			alcDestroyContext(ctx);
		alcCloseDevice(device);

		return;
	}

	Core::Debug->Log("Audio Device: " + std::string(alcGetString(device, ALC_DEVICE_SPECIFIER)) + 
			", " + std::to_string(alIsExtensionPresent("AL_EXT_MCFORMATS")) + "\n");
	AudioEnabled = true;
}


void AL::Close(void)
{
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


void AL::SetCurrentListener(std::shared_ptr<Core::Components::Entity> e)
{
	entity = e;
	lastPosition = glm::vec3(entity->GetMatrix() * glm::vec4(0, 0, 0, 1));
}


void AL::Update()
{
	if (AudioEnabled)
	{
		// Update Listener
		glm::vec3 forward = entity->Forward();
		glm::vec3 up = entity->Up();
		glm::vec3 newPosition = glm::vec3(entity->GetMatrix() * glm::vec4(0, 0, 0, 1));
		glm::vec3 vel = (newPosition - lastPosition) / Core::Time->RealtimeDelta;
		ALfloat ListenerPos[] = { newPosition.x, newPosition.y, newPosition.z };
		ALfloat ListenerVel[] = { vel.x, vel.y, vel.z };
		ALfloat ListenerOri[] = {
			forward.x, forward.y, forward.z,	// Facing Direction
			up.x, up.y, up.z					// Up Direction
		};
		alListenerfv(AL_POSITION, ListenerPos);
		alListenerfv(AL_VELOCITY, ListenerVel);
		alListenerfv(AL_ORIENTATION, ListenerOri);

		alListenerf(AL_GAIN, Volume);

		lastPosition = newPosition;
	}
	else
	{
		alListenerf(AL_GAIN, 0.0f);
	}	
}


void AL::ToggleAudio()
{
	AudioEnabled = !AudioEnabled;
}
void AL::ToggleMusic()
{
	MusicEnabled = !MusicEnabled;
}