#include <Game.h>
#include <Input/Action.h>
#include <Space/TransformIF.h>

namespace Game
{
	class EntityMovementAction : public Core::Input::Action
	{
	protected:
		std::weak_ptr<Core::Space::TransformIF> Target;
		float Velocity;
		bool InRealtime;

		virtual glm::vec3 Direction(std::shared_ptr<Core::Space::TransformIF>& t) = 0;

	public:
		EntityMovementAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime = false);
		virtual ~EntityMovementAction();
		virtual void Perform();
	};


	class CameraUpAction : public EntityMovementAction
	{
	protected:
		virtual glm::vec3 Direction(std::shared_ptr<Core::Space::TransformIF>& t);
	public:
		CameraUpAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime = false);
		virtual ~CameraUpAction();
	};

	class CameraDownAction : public EntityMovementAction
	{
	protected:
		virtual glm::vec3 Direction(std::shared_ptr<Core::Space::TransformIF>& t);
	public:
		CameraDownAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime = false);
		virtual ~CameraDownAction();
	};

	class CameraLeftAction : public EntityMovementAction
	{
	protected:
		virtual glm::vec3 Direction(std::shared_ptr<Core::Space::TransformIF>& t);
	public:
		CameraLeftAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime = false);
		virtual ~CameraLeftAction();
	};

	class CameraRightAction : public EntityMovementAction
	{
	protected:
		virtual glm::vec3 Direction(std::shared_ptr<Core::Space::TransformIF>& t);
	public:
		CameraRightAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime = false);
		virtual ~CameraRightAction();
	};

	class CameraZoomInAction : public EntityMovementAction
	{
	protected:
		virtual glm::vec3 Direction(std::shared_ptr<Core::Space::TransformIF>& t);
	public:
		CameraZoomInAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime = false);
		virtual ~CameraZoomInAction();
	};
	class CameraZoomOutAction : public EntityMovementAction
	{
	protected:
		virtual glm::vec3 Direction(std::shared_ptr<Core::Space::TransformIF>& t);
	public:
		CameraZoomOutAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime = false);
		virtual ~CameraZoomOutAction();
	};


	class CameraRotationAction : public Core::Input::Action
	{
	protected:
		std::weak_ptr<Core::Space::TransformIF> Target;
		float AngularVelocity;

	public:
		CameraRotationAction(std::weak_ptr<Core::Space::TransformIF> target, float angularVelocity);
		virtual ~CameraRotationAction();
		virtual void Perform();
	};

}
