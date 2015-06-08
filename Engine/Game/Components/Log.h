#pragma once

#include <Game.h>

namespace Game
{
	namespace Components
	{		
		class Log : public Core::Patterns::Observable
		{
		private:
			std::vector<std::string> Lines;

		public:
			Log();
			virtual ~Log();

			void Post(std::string line);
			std::string GetLine(size_t i);
			size_t GetSize();
		};
	}
}