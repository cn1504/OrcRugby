#pragma once
#include "Core.h"

namespace Core
{
	namespace Time
	{
		extern float Scale;
		extern float Delta;
		extern float Elapsed;
		extern float RealtimeDelta;
		extern float RealtimeElapsed;
		extern float FPS;
		extern int DrawCallsPerFrame;
		
		void Init();
		void NextUpdate(bool HasFocus);
	}
}
