// Standard Engine Includes
#pragma once
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")	// Hides the console window

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <stack>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <iomanip>
#include <memory>
#include <functional>
#include <chrono>
#include <stdexcept>
#include <thread>
#include <future>
#include <regex>

#define GLEW_STATIC
#define GLEW_MX
#include <GL/glew.h>
#pragma comment(lib, "glew32mxs.lib")
#pragma comment(lib, "opengl32.lib")

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#pragma comment(lib, "libOpenAL32.dll.a")

#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3.lib")

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/norm.hpp>
//#include <glm/gtx/quaternion.hpp>

#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <Bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <Bullet/BulletSoftBody/btSoftBodyHelpers.h>
#pragma comment(lib, "Bullet.lib")

#include <sqlite3.h>
#pragma comment(lib, "SQLite.lib")

#include <zlib.h>
#pragma comment(lib, "zlibstat.lib")

#include <Patterns/Observer.h>
#include <Patterns/Command.h>

#include "Exceptions.h"
#include "Debug/Debug.h"
#include "Debug/Log.h"
#include "Time/Timer.h"

GLEWContext* glewGetContext();

// Shared Interfaces
namespace Core
{
	namespace Assets
	{
		class AssetDB;
		class Database;
		class Font;
		class Text;
		class Texture;
		class Light;
		class AudioFile;
		class Material;
	}

	namespace Audio
	{
		class AL;
	}

	namespace Components
	{
		class Component;
		class Entity;

		class Camera;
		class AudioSource;
		class LightSource;

		namespace Gui
		{
			class Item;
		}
	}

	namespace Input
	{
		class Map;
	}

	namespace Renderers
	{
		class Renderable;
		class Shader; 
		class BufferClearBehavior;
		class RenderBuffer;

		class GuiRenderer;
		class MeshRenderer;
		class LightRenderer;

		class VertexBuffer;
	}

	namespace PhysicsEngine
	{
		class DynamicsWorld;
	}

	namespace Window
	{
		class Window;	
		class WindowAction;
		class State;
	}

	class Timer;
	class Logger;
}


namespace Core
{
	extern std::shared_ptr<Core::Logger> Debug;
	extern std::shared_ptr<Core::Timer> Time;
	extern std::shared_ptr<Core::Assets::Database> Database;
	extern std::shared_ptr<Core::Assets::AssetDB> AssetDB;
	extern std::shared_ptr<Core::Components::Entity> Scene;
	extern std::shared_ptr<Core::Audio::AL> Listener;

	void Init(int argc, char* argv[]);
	void Run();
	void Close();
}

namespace std
{
	std::string to_string(const glm::ivec2& v);
	std::string to_string(const glm::ivec3& v);
	std::string to_string(const glm::ivec4& v);
	std::string to_string(const glm::vec2& v);
	std::string to_string(const glm::vec3& v);
	std::string to_string(const glm::vec4& v);
	std::string to_string(const glm::quat& v);
	std::string to_string(const glm::mat4& v);
	
	glm::ivec2 stoivec2(const std::string& v);
	glm::ivec3 stoivec3(const std::string& v);
	glm::ivec4 stoivec4(const std::string& v);
	glm::vec2 stovec2(const std::string& v);
	glm::vec3 stovec3(const std::string& v);
	glm::vec4 stovec4(const std::string& v);
	glm::quat stoquat(const std::string& v);
	glm::mat4 stomat4(const std::string& v);

	std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &s, char delim);
}

namespace glm
{
	glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);

	const glm::mat4 CubeMapViewMatrices[6] = {
		glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)), // +X
		glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)), // -X
		glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)), // +Y
		glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)), // -Y
		glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)), // +Z
		glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)) // -Z
	};
}
