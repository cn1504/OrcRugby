#pragma once
#include <Core.h>
#include "Map.h"

namespace Core
{
	namespace Input
	{
		extern Map GlobalMap;

		void Init();
		void Update();
	}
}