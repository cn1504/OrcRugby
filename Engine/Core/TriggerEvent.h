#pragma once
#include "Core.h"
#include "Entity.h"


#define TRIGGER_SFX ("SFX")
#define TRIGGER_PLAY_ANIMATION ("PlayAnimation")


namespace Core
{	

	class TriggerEvent
	{
	public:
		std::string Type; 

		virtual ~TriggerEvent() {}
		virtual void Run(Entity* caller) = 0;
		virtual void Load(std::istream& in) = 0;
		virtual void Save(std::ostream& out);
	};

	TriggerEvent* LoadTriggerEvent(std::istream& in);
	TriggerEvent* CreateTriggerEvent(std::string type);
	


	class TriggerEventSFX
		: public TriggerEvent
	{
	public:
		std::string AudioTrack;
		float Volume;

		TriggerEventSFX();
		virtual ~TriggerEventSFX();

		virtual void Run(Entity* caller);
		virtual void Load(std::istream& in);
		virtual void Save(std::ostream& out);

		virtual void PlaySFX(Entity* caller, std::string trackName);
	};



	class TriggerEventPlayAnimation
		: public TriggerEvent
	{
	public:
		std::string AnimationName;
		bool Loop;

		TriggerEventPlayAnimation();
		virtual ~TriggerEventPlayAnimation();

		virtual void Run(Entity* caller);
		virtual void Load(std::istream& in);
		virtual void Save(std::ostream& out);
	};

}