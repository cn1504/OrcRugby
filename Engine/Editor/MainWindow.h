#include <Editor.h>
#include <Window/DefaultWindow.h>
#include <Window/WindowAction.h>
#include <Input/Action.h>
#include <Components/Gui/Item.h>
#include "Grid.h"

namespace Editor
{
	class ItemTranslationAction;
	class ItemRotationAction;
	class EntityMovementAction;
	class EntityRotateLeftAction;
	class CameraRotationAction;

	class ExpandGridAction;

	class MainWindow : public Core::Window::DefaultWindow
	{
	protected:
		std::shared_ptr<Core::Components::Gui::Item> FPSLabel;

		std::shared_ptr<EntityMovementAction> CameraFoward;
		std::shared_ptr<EntityMovementAction> CameraBack;
		std::shared_ptr<EntityMovementAction> CameraLeft;
		std::shared_ptr<EntityMovementAction> CameraRight;
		std::shared_ptr<EntityMovementAction> CameraUp;
		std::shared_ptr<EntityMovementAction> CameraDown;
		std::shared_ptr<CameraRotationAction> CameraMouseLook;
		
		std::shared_ptr<ExpandGridAction> ExpandGrid;
		
	public:
		MainWindow();
		virtual ~MainWindow();
		virtual int Update();
	};


	class NewEditorWindowAction : public Core::Input::Action
	{
	public:
		virtual ~NewEditorWindowAction();
		virtual void Perform();
	};


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

	class EntityMovementUpAction : public EntityMovementAction
	{
	protected:
		virtual glm::vec3 Direction(std::shared_ptr<Core::Space::TransformIF>& t);
	public:
		EntityMovementUpAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime = false);
		virtual ~EntityMovementUpAction();
	};

	class EntityMovementDownAction : public EntityMovementAction
	{
	protected:
		virtual glm::vec3 Direction(std::shared_ptr<Core::Space::TransformIF>& t);
	public:
		EntityMovementDownAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime = false);
		virtual ~EntityMovementDownAction();
	};

	class EntityMovementLeftAction : public EntityMovementAction
	{
	protected:
		virtual glm::vec3 Direction(std::shared_ptr<Core::Space::TransformIF>& t);
	public:
		EntityMovementLeftAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime = false);
		virtual ~EntityMovementLeftAction();
	};

	class EntityMovementRightAction : public EntityMovementAction
	{
	protected:
		virtual glm::vec3 Direction(std::shared_ptr<Core::Space::TransformIF>& t);
	public:
		EntityMovementRightAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime = false);
		virtual ~EntityMovementRightAction();
	};

	class EntityMovementFowardAction : public EntityMovementAction
	{
	protected:
		virtual glm::vec3 Direction(std::shared_ptr<Core::Space::TransformIF>& t);
	public:
		EntityMovementFowardAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime = false);
		virtual ~EntityMovementFowardAction();
	};

	class EntityMovementBackAction : public EntityMovementAction
	{
	protected:
		virtual glm::vec3 Direction(std::shared_ptr<Core::Space::TransformIF>& t);
	public:
		EntityMovementBackAction(std::weak_ptr<Core::Space::TransformIF> target, float velocity, bool inRealtime = false);
		virtual ~EntityMovementBackAction();
	};

	class CameraRotationAction : public Core::Input::Action
	{
	protected:
		std::weak_ptr<Core::Space::TransformIF> Target;
		const glm::vec2* MouseDelta;
		float Sensitivity;

	public:
		CameraRotationAction(std::weak_ptr<Core::Space::TransformIF> target, const glm::vec2* mouseDelta, float sensitivity);
		virtual ~CameraRotationAction();
		virtual void Perform();
	};

	class ItemTranslationAction : public Core::Input::Action
	{
	protected:
		std::weak_ptr<Core::Space::Translatable2D> Target;
		glm::vec2 Velocity;

	public:
		ItemTranslationAction(std::weak_ptr<Core::Space::Translatable2D> target, const glm::vec2& velocity);
		virtual ~ItemTranslationAction();
		virtual void Perform();
	};

	class ItemRotationAction : public Core::Input::Action
	{
	protected:
		std::weak_ptr<Core::Space::Rotatable2D> Target;
		float Velocity;

	public:
		ItemRotationAction(std::weak_ptr<Core::Space::Rotatable2D> target, float velocity);
		virtual ~ItemRotationAction();
		virtual void Perform();
	};

	class EntityRotateLeftAction : public Core::Input::Action
	{
	protected:
		std::weak_ptr<Core::Space::TransformIF> Target;
		float AngularVelocity;

	public:
		EntityRotateLeftAction(std::weak_ptr<Core::Space::TransformIF> target, float angularVelocity);
		virtual ~EntityRotateLeftAction();
		virtual void Perform();
	};
	
	
	class ExpandGridAction : public Core::Input::Action
	{
	protected:
		std::weak_ptr<Editor::Components::Grid> Grid;
		std::weak_ptr<Core::Space::TransformIF> Target;

	public:
		ExpandGridAction(std::weak_ptr<Editor::Components::Grid> grid, std::weak_ptr<Core::Space::TransformIF> target);
		virtual ~ExpandGridAction();
		virtual void Perform();
	};
}