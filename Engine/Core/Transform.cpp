#include "Transform.h"

namespace Core
{

	Transform::Transform()
	{
		Position = glm::vec3(0.0f, 0.0f, 0.0f);
		Rotation = glm::quat();
		Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		Up = glm::vec3(0.0f, 1.0f, 0.0f);
		Forward = glm::vec3(0.0f, 0.0f, 1.0f);
	}

	
	Transform::Transform(float t[10])
	{
		Position = glm::vec3(t[0], t[1], t[2]);
		Rotation = glm::quat(t[3], t[4], t[5], t[6]);
		Scale    = glm::vec3(t[7], t[8], t[9]);
	}


	Transform::~Transform()
	{
	}


	glm::vec3 Transform::WSUp()
	{
		return Rotation * Up;
	}


	glm::vec3 Transform::WSForward()
	{
		return Rotation * Forward;
	}


	glm::mat4 Transform::ToMatrix()
	{
		auto t = glm::translate(glm::mat4(), Position);
		auto r = glm::toMat4(Rotation);
		auto s = glm::scale(Scale);
		
		return t * r * s;
	}

}