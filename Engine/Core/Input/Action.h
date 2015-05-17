#pragma once

#include <Core.h>

namespace Core
{
	namespace Input
	{
		class Action
		{
		public:
			virtual ~Action();
			virtual void Perform() = 0;
		};

		class NullAction : public Action
		{
		public:
			virtual void Perform() { Core::Debug->Log("NullAction Performed."); }
		};
	}
}