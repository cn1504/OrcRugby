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
		private:
			std::vector<std::weak_ptr<Observer>> Observers;

		public:
			virtual ~Observable() {}
			void AddObserver(std::weak_ptr<Observer> o) { Observers.push_back(o); }
			void RemoveObserver(std::shared_ptr<Observer> o)
			{
				for (int i = 0; i < Observers.size(); i++)
				{
					if (Observers[i].lock() == o)
					{
						std::swap(Observers[i], Observers.back());
						Observers.pop_back();
						return;
					}
				}
			}
			void NotifyObservers()
			{
				for (int i = 0; i < Observers.size(); i++)
				{
					if (auto p = Observers[i].lock())
					{
						p->Notify();
					}
					else
					{
						std::swap(Observers[i], Observers.back());
						Observers.pop_back();
						i--;
					}
				}
			}
		};
	}
}