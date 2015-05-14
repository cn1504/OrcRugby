#include "CameraTarget.h"

using namespace Core::Components
{
	
	CameraTarget::CameraTarget(Core::Entity* target)
	{
		Target = target;
	}

	CameraTarget::~CameraTarget() {}

	void CameraTarget::Load()
	{
		TransitionDuration = 0.3f;
		TransitionTimeElapsed = TransitionDuration + 1.0f;
	}
	
	void CameraTarget::Update()
	{
		if (Target == nullptr)
			return;

		if (TransitionTimeElapsed < TransitionDuration)
		{
			TransitionTimeElapsed += Time::RealtimeDelta;
			
			float t = TransitionTimeElapsed / TransitionDuration;
			float y = (glm::cos(t * glm::pi<float>()) + 1.0f) * 0.5f;

			auto targetPosition = Target->Transform.Position;
			targetPosition.y += 1.0f;
			Entity->Transform.Position = OriginalPosition * y + targetPosition * (1.0f - y);
		}
		else
		{
			Entity->Transform.Position = Target->Transform.Position;
			Entity->Transform.Position.y += 1.0f;
		}
	}

	void CameraTarget::SetTarget(Core::Entity* target)
	{
		OriginalPosition = (Target == nullptr) ? Entity->Transform.Position : Target->Transform.Position;
		Target = target;
		TransitionTimeElapsed = 0.0f;
	}

}