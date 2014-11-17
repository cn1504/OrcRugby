#include "OrbitingPosition.h"

namespace Core
{

	OrbitingPosition::OrbitingPosition(Core::Entity* target, glm::vec3 initialVelocity, float acceleration)
	{
		Target = target;
		Velocity = initialVelocity;
		Acceleration = acceleration;
	}


	OrbitingPosition::~OrbitingPosition()
	{
	}

	
	void OrbitingPosition::Load()
	{
		LastTargetPosition = Target->Transform.Position;
		float radius = glm::distance(Entity->Transform.Position, Target->Transform.Position);
		GM = Acceleration / (radius * radius);
	}


	void OrbitingPosition::Update()
	{
		auto deltaP = Target->Transform.Position - LastTargetPosition;
		Entity->Transform.Position += deltaP;
		LastTargetPosition = Target->Transform.Position;

		float radius = glm::distance(Entity->Transform.Position, Target->Transform.Position);
		Acceleration = GM * (radius * radius);

		Velocity += glm::normalize(Target->Transform.Position - Entity->Transform.Position) * Acceleration * Time::Delta;
		Entity->Transform.Position += Velocity * Time::Delta;
	}
	
}