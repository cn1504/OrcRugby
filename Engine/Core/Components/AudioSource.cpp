#include "AudioSource.h"
#include <Assets/AssetDB.h>
#include <Audio/Track.h>
#include <Audio/Listener.h>

using namespace Core::Components;


AudioSource::AudioSource(std::string audioFile)
	: audioFile(audioFile)
{
	auto t = new Core::Audio::Track(audioFile, this, 1.0f, true);
	Core::Listener->PlayTrack(t);
}
AudioSource::~AudioSource()
{
}

void AudioSource::Update()
{

	Entity::Update();
}
