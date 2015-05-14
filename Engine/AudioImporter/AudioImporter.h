#pragma once

#include <Core.h>
#pragma comment(lib, "Core.lib")

#include <sndfile.h>
#pragma comment(lib, "libsndfile-1.lib")

namespace AudioImporter
{
	extern std::shared_ptr<Core::Logger> out;
}