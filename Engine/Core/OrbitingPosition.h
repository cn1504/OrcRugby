#pragma once
#include "Core.h"
#include "Component.h"

namespace Core
{

	class OrbitingPosition
		: public Component
	{
	private:
		Core::Entity* Target;
		glm::vec2 AngularSpeed;

		float GM;

		glm::vec3 Velocity;
		float Acceleration;

		glm::vec3 LastTargetPosition;

	public:
		OrbitingPosition(Core::Entity* target, glm::vec3 initialVelocity, float acceleration);
		virtual ~OrbitingPosition();

		virtual void Load();
		virtual void Update();
	};

}