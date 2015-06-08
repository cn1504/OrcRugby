#include "Log.h"

using namespace Game::Components;

Log::Log() {}
Log::~Log() {}

void Log::Post(std::string line)
{
	Lines.push_back(line);
	NotifyObservers();
}

std::string Log::GetLine(size_t i) { return (i < Lines.size()) ? Lines[i] : ""; }
size_t Log::GetSize() { return Lines.size(); }
