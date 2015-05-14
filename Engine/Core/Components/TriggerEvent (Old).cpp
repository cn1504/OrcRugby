#include "TriggerEvent.h"

#include <Assets/Assets.h>
#include <Audio/Listener.h>
#include "SkeletalAnimation.h"

using namespace Core::Components;


void TriggerEvent::Save(std::ostream& out)
{
	out << Type << std::endl;
}

TriggerEvent* LoadTriggerEvent(std::istream& in)
{
	std::string type;
	std::getline(in, type);

	TriggerEvent* e = CreateTriggerEvent(type);

	if (e != nullptr)
		e->Load(in);
	return e;
}

TriggerEvent* CreateTriggerEvent(std::string type)
{
	TriggerEvent* e = nullptr;

	if (type == TRIGGER_SFX)
	{
		e = new TriggerEventSFX;
	}
	else if (type == TRIGGER_PLAY_ANIMATION)
	{
		e = new TriggerEventPlayAnimation;
	}
	else
	{
		Debug::Log("Invalid Trigger Event Type: " + type);
	}

	return e;
}





TriggerEventSFX::TriggerEventSFX() 
{
	Type = TRIGGER_SFX;
	AudioTrack = "";
	Volume = 1.0f;
}

TriggerEventSFX::~TriggerEventSFX() {}

void TriggerEventSFX::Run(Entity* caller)
{
	PlaySFX(caller, AudioTrack);
}

void TriggerEventSFX::PlaySFX(Entity* caller, std::string trackName)
{
	if (trackName != "")
		AudioListener.PlayTrack(new Track(Assets::LoadAudioFile(trackName), caller, Volume, false));
}
	
void TriggerEventSFX::Load(std::istream& in)
{
	std::string input;

	std::getline(in, AudioTrack);
	std::getline(in, input);
	Volume = std::stof(input);
}

void TriggerEventSFX::Save(std::ostream& out)
{
	TriggerEvent::Save(out);

	out << AudioTrack << std::endl;
	out << Volume << std::endl;
}





TriggerEventPlayAnimation::TriggerEventPlayAnimation()
{
	Type = TRIGGER_PLAY_ANIMATION;
	AnimationName = "Idle";
	Loop = true;
}

TriggerEventPlayAnimation::~TriggerEventPlayAnimation() {}

void TriggerEventPlayAnimation::Run(Entity* caller)
{
	auto sa = caller->GetComponent<SkeletalAnimation>();
	if (sa != nullptr)
	{
		sa->PlayAnimation(AnimationName, Loop);
	}
}

void TriggerEventPlayAnimation::Load(std::istream& in)
{
	std::string input;

	std::getline(in, AnimationName);
	std::getline(in, input);
	Loop = (std::stoi(input) > 0);
}

void TriggerEventPlayAnimation::Save(std::ostream& out)
{
	TriggerEvent::Save(out);

	out << AnimationName << std::endl;
	out << ((Loop) ? "1" : "0") << std::endl;
}
