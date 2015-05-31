#pragma once

#include <memory>

namespace Core
{
	namespace Patterns
	{
		class Observer
		{
		public:
			virtual ~Observer() {}
			virtual void Notify() = 0;
		};

		class Observable
		{
		public:
			virtual ~Observable() {}
			virtual void AddObserver(std::weak_ptr<Observer> o) = 0;
		};
	}
}