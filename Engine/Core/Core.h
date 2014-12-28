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

#include <tinydir.h>

#define GLEW_STATIC
#include <GL/glew.h>
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#pragma comment(lib, "libOpenAL32.dll.a")


#include <libtexture.h>
#pragma comment(lib, "libtexture.lib")

#include <sndfile.h>
#pragma comment(lib, "libsndfile-1.lib")


#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3dll.lib")

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/norm.hpp>
//#include <glm/gtx/quaternion.hpp>


#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <Bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <Bullet/BulletSoftBody/btSoftBodyHelpers.h>
#pragma comment(lib, "Bullet.lib")

#include "json.h"

#include "Exceptions.h"
#include "Settings.h"
#include "Debug.h"
#include "Time.h"

namespace Core
{
	// Forward Declarations
	class Entity;
	class Component;
	class Renderable;
	class Window;
	class Scene;
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
}

glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
