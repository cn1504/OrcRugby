#include "Camera.h"

using namespace Core::Components;


Camera::Camera()
{
}
			
Camera::~Camera() {}
			
void Camera::Load()
{

}
			
void Camera::Update()
{
	Entity::Update();

	View = glm::lookAt(GetPosition(), GetPosition() + Forward(), Up());
}

void Camera::UpdateProjection(float fovy, float aspectRatio, float minDrawDistance, float maxDrawDistance)
{
	Projection = glm::perspective(fovy, aspectRatio, minDrawDistance, maxDrawDistance);
}

const glm::mat4& Camera::GetViewMatrix()
{
	return View;
}

const glm::mat4& Camera::GetProjectionMatrix()
{
	return Projection;
}
