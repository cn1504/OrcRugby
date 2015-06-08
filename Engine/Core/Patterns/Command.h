#pragma once

namespace Core
{
	namespace Patterns
	{
		class Command
		{
		public:
			virtual ~Command() {}
			virtual void Execute() = 0;
		};
	}
}
