#include "Camera.h"

namespace Core
{

	Camera::Camera()
	{
	}


	Camera::~Camera()
	{
	}


	void Camera::Load()
	{
	}


	void Camera::Update()
	{
	}


	void Camera::UpdateProjection() {
		projection = glm::perspective(
			Settings::Video::FOVY,
			((float)Settings::Window::Width) / ((float)Settings::Window::Height),
			Settings::Video::MinDrawDistance,
			Settings::Video::MaxDrawDistance);
	}


	glm::mat4 Camera::GetViewMatrix()
	{
		glm::mat4 view = glm::lookAt(
			Entity->Transform.Position,
			Entity->Transform.Position + Entity->Transform.Rotation * Entity->Transform.Forward,
			Entity->Transform.Rotation * Entity->Transform.Up);
		return view;
	}


	glm::mat4 Camera::GetProjectionMatrix()
	{
		return projection;
	}

}