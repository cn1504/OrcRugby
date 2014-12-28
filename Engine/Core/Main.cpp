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


namespace std
{
	std::string to_string(const glm::ivec2& v)
	{
		return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
	}

	std::string to_string(const glm::ivec3& v)
	{
		return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
	}

	std::string to_string(const glm::ivec4& v)
	{
		return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ", " + std::to_string(v.w) + ")";
	}

	std::string to_string(const glm::vec2& v)
	{
		return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
	}

	std::string to_string(const glm::vec3& v)
	{
		return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
	}

	std::string to_string(const glm::vec4& v)
	{
		return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ", " + std::to_string(v.w) + ")";
	}

	std::string to_string(const glm::quat& v)
	{
		return "(" + std::to_string(v.w) + ", " + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
	}

	std::string to_string(const glm::mat4& v)
	{
		return "[" + std::to_string(v[0]) + ", " + std::to_string(v[1]) + ", " + std::to_string(v[2]) + ", " + std::to_string(v[3]) + "]";
	}
	

	glm::ivec2 stoivec2(const std::string& v)
	{
		glm::ivec2 result;

		auto s = v.find("(");
		if (s == std::string::npos)
			return result;

		auto c1 = v.find(",", s + 1);
		if (c1 == std::string::npos)
			return result;

		auto e = v.find(")", c1 + 1);
		if (e == std::string::npos)
			return result;
		
		result.x = std::stoi(v.substr(s + 1, c1));
		result.y = std::stoi(v.substr(c1 + 1, e));	

		return result;
	}

	glm::ivec3 stoivec3(const std::string& v)
	{
		glm::ivec3 result;

		auto s = v.find("(");
		if (s == std::string::npos)
			return result;

		auto c1 = v.find(",", s + 1);
		if (c1 == std::string::npos)
			return result;

		auto c2 = v.find(",", c1 + 1);
		if (c2 == std::string::npos)
			return result;

		auto e = v.find(")", c2 + 1);
		if (e == std::string::npos)
			return result;

		result.x = std::stoi(v.substr(s + 1, c1));
		result.y = std::stoi(v.substr(c1 + 1, c2));
		result.z = std::stoi(v.substr(c2 + 1, e));

		return result;
	}
	
	glm::ivec4 stoivec4(const std::string& v)
	{
		glm::ivec4 result;

		auto s = v.find("(");
		if (s == std::string::npos)
			return result;

		auto c1 = v.find(",", s + 1);
		if (c1 == std::string::npos)
			return result;

		auto c2 = v.find(",", c1 + 1);
		if (c2 == std::string::npos)
			return result;

		auto c3 = v.find(",", c2 + 1);
		if (c3 == std::string::npos)
			return result;

		auto e = v.find(")", c3 + 1);
		if (e == std::string::npos)
			return result;

		result.x = std::stoi(v.substr(s + 1, c1));
		result.y = std::stoi(v.substr(c1 + 1, c2));
		result.z = std::stoi(v.substr(c2 + 1, c3));
		result.w = std::stoi(v.substr(c3 + 1, e));

		return result;
	}

	glm::vec2 stovec2(const std::string& v)
	{
		glm::vec2 result;

		auto s = v.find("(");
		if (s == std::string::npos)
			return result;

		auto c1 = v.find(",", s + 1);
		if (c1 == std::string::npos)
			return result;

		auto e = v.find(")", c1 + 1);
		if (e == std::string::npos)
			return result;

		result.x = std::stof(v.substr(s + 1, c1));
		result.y = std::stof(v.substr(c1 + 1, e));

		return result;
	}

	glm::vec3 stovec3(const std::string& v)
	{
		glm::vec3 result;

		auto s = v.find("(");
		if (s == std::string::npos)
			return result;

		auto c1 = v.find(",", s + 1);
		if (c1 == std::string::npos)
			return result;

		auto c2 = v.find(",", c1 + 1);
		if (c2 == std::string::npos)
			return result;

		auto e = v.find(")", c2 + 1);
		if (e == std::string::npos)
			return result;

		result.x = std::stof(v.substr(s + 1, c1));
		result.y = std::stof(v.substr(c1 + 1, c2));
		result.z = std::stof(v.substr(c2 + 1, e));

		return result;
	}

	glm::vec4 stovec4(const std::string& v)
	{
		glm::vec4 result;

		auto s = v.find("(");
		if (s == std::string::npos)
			return result;

		auto c1 = v.find(",", s + 1);
		if (c1 == std::string::npos)
			return result;

		auto c2 = v.find(",", c1 + 1);
		if (c2 == std::string::npos)
			return result;

		auto c3 = v.find(",", c2 + 1);
		if (c3 == std::string::npos)
			return result;

		auto e = v.find(")", c3 + 1);
		if (e == std::string::npos)
			return result;

		result.x = std::stof(v.substr(s + 1, c1));
		result.y = std::stof(v.substr(c1 + 1, c2));
		result.z = std::stof(v.substr(c2 + 1, c3));
		result.w = std::stof(v.substr(c3 + 1, e));

		return result;
	}

	glm::quat stoquat(const std::string& v)
	{
		glm::quat result;

		auto s = v.find("(");
		if (s == std::string::npos)
			return result;

		auto c1 = v.find(",", s + 1);
		if (c1 == std::string::npos)
			return result;

		auto c2 = v.find(",", c1 + 1);
		if (c2 == std::string::npos)
			return result;

		auto c3 = v.find(",", c2 + 1);
		if (c3 == std::string::npos)
			return result;

		auto e = v.find(")", c3 + 1);
		if (e == std::string::npos)
			return result;

		result.w = std::stof(v.substr(s + 1, c1));
		result.x = std::stof(v.substr(c1 + 1, c2));
		result.y = std::stof(v.substr(c2 + 1, c3));
		result.z = std::stof(v.substr(c3 + 1, e));

		return result;
	}

	glm::mat4 stomat4(const std::string& v)
	{
		glm::mat4 result;
		
		auto s = v.find("[");
		if (s == std::string::npos)
			return result;

		auto v1 = v.find(")", s + 1);
		if (v1 == std::string::npos)
			return result;

		auto c1 = v.find(",", v1 + 1);
		if (c1 == std::string::npos)
			return result;

		auto v2 = v.find(")", c1 + 1);
		if (v2 == std::string::npos)
			return result;

		auto c2 = v.find(",", v2 + 1);
		if (c2 == std::string::npos)
			return result;

		auto v3 = v.find(")", c2 + 1);
		if (v3 == std::string::npos)
			return result;

		auto c3 = v.find(",", v3 + 1);
		if (c3 == std::string::npos)
			return result;

		auto v4 = v.find(")", c2 + 1);
		if (v3 == std::string::npos)
			return result;

		auto e = v.find("]", v4 + 1);
		if (e == std::string::npos)
			return result;

		result[0] = std::stovec4(v.substr(s + 1, c1));
		result[1] = std::stovec4(v.substr(c1 + 1, c2));
		result[2] = std::stovec4(v.substr(c2 + 1, c3));
		result[3] = std::stovec4(v.substr(c3 + 1, e));

		return result;
	}
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