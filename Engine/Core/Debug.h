#pragma once
#include "Core.h"

namespace Core
{
	namespace Debug
	{
		namespace {
			std::string logFilename = "Log.txt";
			std::fstream logFile;
		}

		void Init();
		void Log(std::string message);
		void Close();

		void Error(std::string error_message);
		void GLError(std::string error_message);

		void error_callback(int error, const char* description);
	}
}