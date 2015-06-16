#include "AudioSource.h"
#include <Assets/AssetDB.h>
#include <Audio/Listener.h>
#include <Assets/AudioFile.h>

using namespace Core::Components;


AudioSource::AudioSource(std::string audioFile)
	: File(audioFile)
{
	// Load file
	alGenSources(1, &Source);

	ElapsedTime = 0.0f;
	Duration = 0.0f; 
	Volume = 1.0f;
	UpdateBehavior = std::make_unique<AudioSourceUpdateBehavior>(this);

	Play();
}
AudioSource::~AudioSource()
{
	// Finish
	alSourceStop(Source);
	alDeleteSources(1, &Source);
}

void AudioSource::Update()
{
	Entity::Update();

	UpdateBehavior->Update();

	auto af = Core::AssetDB->GetAudioFile(File);

	float vol = Core::Listener->GetVolume();
	if (af->type == Core::Assets::AudioFile::Type::SOUND_EFFECT)
		vol *= Core::Listener->GetSFXVolume();
	else if (af->type == Core::Assets::AudioFile::Type::AMBIENT)
		vol *= Core::Listener->GetAmbientVolume();
	else if (af->type == Core::Assets::AudioFile::Type::VOICE)
		vol *= Core::Listener->GetVoiceVolume();
	else if (af->type == Core::Assets::AudioFile::Type::MUSIC)
		vol *= Core::Listener->GetMusicVolume();
	alSourcef(Source, AL_GAIN, Volume * vol);

	auto newPosition = glm::vec3(GetMatrix() * glm::vec4(0, 0, 0, 1));
	alSourcefv(Source, AL_POSITION, glm::value_ptr(newPosition));
	alSourcefv(Source, AL_VELOCITY, glm::value_ptr((newPosition - Position) * Core::Time->RealtimeDelta));
	Position = newPosition;

	ElapsedTime += Core::Time->RealtimeDelta;
	if (af->loop)
		while (ElapsedTime > Duration)
			ElapsedTime -= Duration;
}

void AudioSource::Play()
{
	auto af = Core::AssetDB->GetAudioFile(File);
	ElapsedTime = 0.0f;
	Duration = af->duration;
	alSourcei(Source, AL_BUFFER, af->buffer);
	alSourcei(Source, AL_LOOPING, af->loop);

	alSourcePlay(Source);
}

void AudioSource::Stop()
{
	alSourceStop(Source);
}

void AudioSource::FadeToNewFile(std::string file, float duration)
{
	UpdateBehavior = std::make_unique<AudioSourceFadeToNew>(this, file, duration);
}

AudioSourceFadeToNew::AudioSourceFadeToNew(AudioSource* as, std::string nextFile, float duration)
	: AudioSourceUpdateBehavior(as), NextFile(nextFile), Duration(duration), Elapsed(0.0f)
{
	InitialVolume = AS->Volume;
}
void AudioSourceFadeToNew::Update()
{
	Elapsed += Core::Time->RealtimeDelta;

	if (Elapsed > Duration)
	{
		AS->Volume = 1.0f;
		AS->UpdateBehavior = std::make_unique<AudioSourceUpdateBehavior>(AS);
	}
	else if (Elapsed > 0.5f * Duration)
	{
		AS->Stop();
		AS->File = NextFile;
		AS->Play();
		AS->Volume = (Elapsed - 0.5f * Duration) / (0.5f * Duration);
	}
	else
	{
		AS->Volume = InitialVolume * (1.0f - (Elapsed / (0.5f * Duration)));
	}
}