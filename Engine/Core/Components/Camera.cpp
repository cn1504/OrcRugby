#include "Camera.h"

using namespace Core::Components;


Camera::Camera()
{
	View = glm::lookAt(GetPosition(), GetPosition() + Forward(), Up());
	InverseView = glm::inverse(View);

	UpdateProjection(60.0f, 16.0f / 9.0f, 1.0f, 100.0f);

	ViewProjection = Projection * View;
	InverseViewProjection = glm::inverse(ViewProjection);
}
			
Camera::~Camera() {}
			
void Camera::Load()
{

}
			
void Camera::Update()
{
	Entity::Update();

	View = glm::lookAt(GetPosition(), GetPosition() + Forward(), Up());
	InverseView = glm::inverse(View);
	ViewProjection = Projection * View;
	InverseViewProjection = glm::inverse(ViewProjection);
}

void Camera::UpdateProjection(float fovy, float aspectRatio, float minDrawDistance, float maxDrawDistance)
{
	Projection = glm::perspective(fovy, aspectRatio, minDrawDistance, maxDrawDistance);
	InverseProjection = glm::inverse(Projection);
}

void Camera::GetWSFrustumCorners(std::vector<glm::vec3>& corners)
{
	corners.clear();

	// homogeneous corner coords
	glm::vec4 hcorners[8];
	// near
	hcorners[0] = glm::vec4(-1, 1, 1, 1);
	hcorners[1] = glm::vec4(1, 1, 1, 1);
	hcorners[2] = glm::vec4(1, -1, 1, 1);
	hcorners[3] = glm::vec4(-1, -1, 1, 1);
	// far
	hcorners[4] = glm::vec4(-1, 1, -1, 1);
	hcorners[5] = glm::vec4(1, 1, -1, 1);
	hcorners[6] = glm::vec4(1, -1, -1, 1);
	hcorners[7] = glm::vec4(-1, -1, -1, 1);

	for (int i = 0; i < 8; i++) {
		hcorners[i] = InverseViewProjection * hcorners[i];
		hcorners[i] /= hcorners[i].w;

		corners.push_back(glm::vec3(hcorners[i]));
	}
}

glm::vec3 Camera::GetGridLocation()
{
	auto pos = GetPosition();
	auto f = Forward();
	auto t = -pos.y / f.y;
	return glm::vec3(pos.x + f.x * t, 0, pos.z + f.z * t);
}