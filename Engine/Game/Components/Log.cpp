#include "Log.h"

using namespace Game::Components;

Log::Log() {}
Log::~Log() {}

void Log::AddObserver(std::weak_ptr<Core::Patterns::Observer> o)
{
	Observers.push_back(std::move(o));
}
void Log::Post(std::string line)
{
	Lines.push_back(line);
	NotifyObservers();
}

std::string Log::GetLine(size_t i) { return (i < Lines.size()) ? Lines[i] : ""; }
size_t Log::GetSize() { return Lines.size(); }

void Log::NotifyObservers()
{
	for (size_t i = 0; i < Observers.size(); i++)
	{
		if (auto o = Observers[i].lock())
		{
			o->Notify();
		}
		else
		{
			std::swap(Observers[i], Observers.back());
			Observers.pop_back();
			i--;
		}
	}
}