#include "AudioSource.h"
#include <Assets/AssetDB.h>

using namespace Core::Components;


AudioSource::AudioSource(std::string audioFile)
	: audioFile(audioFile)
{
}
AudioSource::~AudioSource()
{
}

void AudioSource::Update()
{

	Entity::Update();
}
