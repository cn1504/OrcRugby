#include "Log.h"

using namespace Core;

Logger::Logger(std::string filepath)
{
	logFile.open(filepath, std::ios::out);
}
Logger::~Logger()
{
	logFile.close();
}

void Logger::Log(std::string message)
{
	logFile << message << std::endl;
}

void Logger::Error(std::string error_message)
{
	Log(error_message);
	Close();
	exit(EXIT_FAILURE);
}

void Logger::GLError(std::string error_message)
{
	const GLenum ErrorValue = glGetError();

	if (ErrorValue != GL_NO_ERROR) {
		Log("GL Error: \"" + std::to_string(ErrorValue) + "\": " + error_message);
		Close();
		exit(EXIT_FAILURE);
	}
}
