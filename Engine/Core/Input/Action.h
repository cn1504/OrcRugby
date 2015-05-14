#pragma once

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
	}
}