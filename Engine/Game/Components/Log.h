#pragma once

#include <Game.h>

namespace Game
{
	namespace Components
	{		
		class Log : public Core::Patterns::Observable
		{
		private:
			std::vector<std::weak_ptr<Core::Patterns::Observer>> Observers;
			std::vector<std::string> Lines;

			void NotifyObservers();

		public:
			Log();
			virtual ~Log();

			virtual void AddObserver(std::weak_ptr<Core::Patterns::Observer> o);
			void Post(std::string line);
			std::string GetLine(size_t i);
			size_t GetSize();
		};
	}
}