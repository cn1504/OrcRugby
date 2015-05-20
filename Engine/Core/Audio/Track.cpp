#include "Track.h"
#include "Listener.h"
#include <Assets/AssetDB.h>

using namespace Core::Audio;

Track::Track(std::string file, Core::Components::Entity* bond, float volume, bool loop)
{
	this->file = file;
	this->bond = bond;
	this->volume = volume;
	this->loop = loop;

	// Load file
	alGenSources(1, &source);
	
	elapsedTime = 0.0f;
	duration = 0.0f;

	Play();
}


Track::Track(std::string file, glm::vec3 position, glm::vec3 velocity, float volume, bool loop)
{
	this->file = file;
	this->bond = nullptr;
	this->volume = volume;
	this->loop = loop;

	alGenSources(1, &source);
	
	elapsedTime = 0.0f;
	duration = 0.0f;
	this->position = position;
	this->velocity = velocity;

	Play();
}


Track::~Track()
{
	// Finish
	alSourceStop(source);

	alDeleteSources(1, &source);
}


void Track::Update()
{
	auto af = Core::AssetDB->GetAudioFile(file);

	float volume = Core::Listener->GetVolume();
	if (af->type == Core::Assets::AudioFile::Type::SOUND_EFFECT)
		volume *= Core::Listener->GetSFXVolume();
	else if (af->type == Core::Assets::AudioFile::Type::AMBIENT)
		volume *= Core::Listener->GetAmbientVolume();
	else if (af->type == Core::Assets::AudioFile::Type::VOICE)
		volume *= Core::Listener->GetVoiceVolume();
	else if (af->type == Core::Assets::AudioFile::Type::MUSIC)
		volume *= Core::Listener->GetMusicVolume();

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
	auto af = Core::AssetDB->GetAudioFile(file);
	duration = af->duration;
	alSourcei(source, AL_BUFFER, af->buffer);
	alSourcef(source, AL_PITCH, 1.0f);
	alSourcei(source, AL_LOOPING, loop);
	Update();

	alSourcePlay(source);
}


bool Track::HasFinished()
{
	return elapsedTime > duration;
}