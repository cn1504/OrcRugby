#pragma once
#include "Core.h"
#include "Component.h"
#include "CharacterStats.h"
#include "Input.h"

namespace Core
{
	class RigidBody;
	class SkeletalAnimation;

	class Character
		: public Component
	{
	protected:
		RigidBody* RB;
		SkeletalAnimation* SA;
		CharacterStats* Stats;
		
		float RunSpeed;
		float WalkSpeed;
		float DodgeSpeed;
		
		Input::MovementMode MoveMode;
		glm::vec3 MoveDirection;

		float InputLockElapsed;
		float InputLockDuration;

	public:
		Character(Core::Entity* e, std::string name);
		virtual ~Character();

		virtual void Load();
		virtual void Update();

		virtual void Save();

		void StopMoving();
		void Move(Input::MovementMode mode, const glm::vec2& moveDirection);
		void Dodge(const glm::vec2& direction);

		void PrimaryAttack(const glm::vec2& direction);
		void SecondaryAttack(const glm::vec2& direction);

		void LockInput(float duration);
		bool IsControlEnabled();

		// Full Access Data
		std::string Name;
		float Height;
	};

}