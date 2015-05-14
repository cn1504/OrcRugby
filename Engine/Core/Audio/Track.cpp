#include "Track.h"
#include "Listener.h"

using namespace Core::Audio;

Track::Track(Core::Assets::AudioFile* file, Core::Components::Entity* bond, float volume, bool loop)
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


Track::Track(Core::Assets::AudioFile* file, glm::vec3 position, glm::vec3 velocity, float volume, bool loop)
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
	if (file->type == Core::Assets::AudioFile::Type::SOUND_EFFECT)
		volume = Core::Listener->GetSFXVolume();
	else if (file->type == Core::Assets::AudioFile::Type::AMBIENT)
		volume = Core::Listener->GetAmbientVolume();
	else if (file->type == Core::Assets::AudioFile::Type::VOICE)
		volume = Core::Listener->GetVoiceVolume();
	else if (file->type == Core::Assets::AudioFile::Type::MUSIC)
		volume = Core::Listener->GetMusicVolume();

	alSourcef(source, AL_GAIN, this->volume * volume);

	if (bond != nullptr)
	{
		position = glm::vec3(bond->GetMatrix() * glm::vec4(0,0,0,1));
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	alSourcefv(source, AL_POSITION, glm::value_ptr(position));
	alSourcefv(source, AL_VELOCITY, glm::value_ptr(velocity));

	elapsedTime += Core::Time->RealtimeDelta;
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