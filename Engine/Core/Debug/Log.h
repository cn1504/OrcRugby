#pragma once

#include <Core.h>

namespace Core
{
	class Logger
	{
	private:
		std::ofstream logFile;

	public:
		Logger(std::string filepath);
		~Logger();

		void Log(std::string message);

		void Error(std::string error_message);
		void GLError(std::string error_message);
	};
}