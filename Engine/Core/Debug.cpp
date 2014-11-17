#include "Debug.h"
#include "AudioListener.h"

namespace Core
{
	namespace Debug {
		void Init() {
			logFile.open(logFilename, std::ios::out);
		}

		void Error(std::string error_message)
		{
			Log(error_message);
			Close();
			exit(EXIT_FAILURE);
		}

		void GLError(std::string error_message)
		{
			const GLenum ErrorValue = glGetError();

			if (ErrorValue != GL_NO_ERROR) {
				Log("GL Error: \"" + std::to_string(ErrorValue) + "\": " + error_message);
				Close();
				exit(EXIT_FAILURE);
			}
		}

		void Log(std::string message) {
			logFile << message << std::endl;
		}

		void Close() 
		{
			AudioListener.Close();
			glfwTerminate();
			Settings::Close();
			logFile.close();
		}

		void error_callback(int error, const char* description) {
			Error(description);
		}
	}
}
