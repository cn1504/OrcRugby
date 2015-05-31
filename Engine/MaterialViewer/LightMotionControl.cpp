#include "LightMotionControl.h"

using namespace MaterialViewer;

LightMotionControl::LightMotionControl()
{

}
LightMotionControl::~LightMotionControl() {}
		
void LightMotionControl::Update()
{
	const float T = (2.0f * glm::pi<float>()) / 4.0f;
	const float R = 2.0f;

	Translate(glm::vec3(R * glm::cos(Core::Time->Elapsed * T), R * glm::sin(Core::Time->Elapsed * T), -R) - GetPosition());

	Entity::Update();
}