#include "CameraControls.h"

using namespace Game;


EntityMovementAction::EntityMovementAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime)
{
	Target = std::move(target);
	Velocity = velocity;
	InRealtime = inRealtime;
}
EntityMovementAction::~EntityMovementAction() {}
void EntityMovementAction::Perform()
{
	if (auto a = Target.lock())
	{
		a->Translate(Direction(a) * Velocity * (InRealtime ? Core::Time->RealtimeDelta : Core::Time->Delta));
	}
	else
	{
		delete this;
	}
}


CameraUpAction::CameraUpAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime)
	: EntityMovementAction(target, velocity, inRealtime) {}
CameraUpAction::~CameraUpAction() {}
glm::vec3 CameraUpAction::Direction(std::shared_ptr<Core::Space::TransformIF>& t) { return -glm::cross(t->Right(), glm::vec3(0,1,0)); }

CameraDownAction::CameraDownAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime)
	: EntityMovementAction(target, velocity, inRealtime) {}
CameraDownAction::~CameraDownAction() {}
glm::vec3 CameraDownAction::Direction(std::shared_ptr<Core::Space::TransformIF>& t) { return glm::cross(t->Right(), glm::vec3(0, 1, 0)); }

CameraLeftAction::CameraLeftAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime)
	: EntityMovementAction(target, velocity, inRealtime) {}
CameraLeftAction::~CameraLeftAction() {}
glm::vec3 CameraLeftAction::Direction(std::shared_ptr<Core::Space::TransformIF>& t) { return -t->Right(); }

CameraRightAction::CameraRightAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime)
	: EntityMovementAction(target, velocity, inRealtime) {}
CameraRightAction::~CameraRightAction() {}
glm::vec3 CameraRightAction::Direction(std::shared_ptr<Core::Space::TransformIF>& t) { return t->Right(); }

CameraZoomInAction::CameraZoomInAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime)
	: EntityMovementAction(target, velocity, inRealtime) {}
CameraZoomInAction::~CameraZoomInAction() {}
glm::vec3 CameraZoomInAction::Direction(std::shared_ptr<Core::Space::TransformIF>& t) { return t->Forward(); }

CameraZoomOutAction::CameraZoomOutAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime)
	: EntityMovementAction(target, velocity, inRealtime) {}
CameraZoomOutAction::~CameraZoomOutAction() {}
glm::vec3 CameraZoomOutAction::Direction(std::shared_ptr<Core::Space::TransformIF>& t) { return -t->Forward(); }


CameraRotationAction::CameraRotationAction(std::weak_ptr<Core::Space::TransformIF> target, float angularVelocity) 
	: Target(target), AngularVelocity(angularVelocity) {}
CameraRotationAction::~CameraRotationAction() {}
void CameraRotationAction::Perform()
{
	if (auto a = Target.lock())
	{
		auto pos = a->GetPosition();
		auto f = a->Forward();
		auto t = -pos.y / f.y;
		auto anchor = glm::vec3(pos.x + f.x * t, 0, pos.z + f.z * t);
		auto rot = Core::Time->RealtimeDelta * AngularVelocity;
		a->Rotate(glm::vec3(0, rot, 0));
		a->UpdateMatrix();
		auto f2 = a->Forward();
		auto t2 = -pos.y / f2.y;
		auto p2 = glm::vec3(pos.x + f2.x * t, 0, pos.z + f2.z * t);
		a->Translate(anchor - p2);
	}
	else
	{
		delete this;
	}
}
