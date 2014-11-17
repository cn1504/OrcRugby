#include "Core.h"
#include "Window.h"
#include "AudioListener.h"

int main(int argc, char* argv[])
{
	try {
		Core::Debug::Init();

		// Initialize GLFW
		glfwSetErrorCallback(Core::Debug::error_callback);
		if (!glfwInit())
			Core::Debug::Error("GLFW failed to initialize.");


		Core::Settings::Init(argc, argv);
		Core::Time::Init();
		Core::AudioListener.Init();


		if (Core::Settings::Misc::VerboseLogging) {
			std::stringstream msg;

			// Log display information
			Core::Debug::Log("Available Monitors:");
			int monitorCount;
			GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
			for (int i = 0; i < monitorCount; i++) {
				msg << "\t" << (i + 1) << ". " << glfwGetMonitorName(monitors[i]);
				const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
				msg << " - " << mode->width << "x" << mode->height << "x" << mode->redBits * 4 << " @ " << mode->refreshRate << "Hz" << std::endl;
			}
			Core::Debug::Log(msg.str());

			Core::Debug::Log("");
			msg.str("");
		}

		// Open the main window
		Core::Window* mainWindow = new Core::Window();
		delete mainWindow;

		Core::Debug::Close();
	}
	catch (std::runtime_error &e) {
		Core::Debug::Log(e.what());
		Core::Debug::Close();
		exit(EXIT_FAILURE);
	}
	return 0;
}


glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest)
{
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	float cosTheta = glm::dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f){
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);
	
		rotationAxis = glm::normalize(rotationAxis);
		return glm::angleAxis(180.0f, rotationAxis);
	}

	rotationAxis = glm::cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
		);

}