#include "Character.h"
#include <Assets/Assets.h>
#include <Scene/Entity.h>
#include "SkeletalAnimation.h"
#include "RigidBody.h"
#include "TriggerEvent.h"

#define MAX_VELOCITY_DELTA (10.0f)

using namespace Core::Components;


Character::Character(Core::Entity* e, std::string name)
{
	Name = name;
	Height = 1.8f;
	DodgeSpeed = 10.0f; 
	RunSpeed = 5.0f;
	WalkSpeed = 2.3f;
	MoveMode = Input::MovementMode::Idle;
	InputLockElapsed = 0.0f;
	InputLockDuration = 0.1f;
		
	if (Settings::Misc::VerboseLogging)
		Debug::Log("Loading character: " + name);

	std::ifstream file("Resources/Characters/" + name);
	if (!file)
	{
		Debug::Error("Invalid character: " + name);
	}

	std::string input;
	std::string input2;
	std::string input3;
	std::string input4;

	std::getline(file, input);
	Height = std::stof(input);

	std::getline(file, input);
	float scale = std::stof(input);
	e->Transform.Scale = glm::vec3(scale, scale, scale);
		
	std::getline(file, input);
	std::getline(file, input2);
	auto anim = new SkeletalAnimation(input, input2);

	int meshCount = 0;
	std::getline(file, input);
	meshCount = std::stoi(input);

	for (int i = 0; i < meshCount; i++)
	{
		std::getline(file, input);
		std::getline(file, input2); 
		Assets::LoadMesh(input);
		anim->AddMesh(input, input2);
	}

	int animationCount = 0;
	std::getline(file, input);
	animationCount = std::stoi(input);

	for (int i = 0; i < animationCount; i++)
	{
		std::getline(file, input);
		std::getline(file, input2);
		std::getline(file, input3);
		std::getline(file, input4);
		anim->AddAnimation(input, new Core::Animation(std::stoi(input2), std::stoi(input3), std::stof(input4)));
	}
		
	int animationEventCount = 0;
	std::getline(file, input);
	animationEventCount = std::stoi(input);

	for (int i = 0; i < animationEventCount; i++)
	{
		std::getline(file, input);
		anim->AddFrameEvent(std::stoi(input), LoadTriggerEvent(file));
	}

	e->AddComponent(anim);
	SA = anim;

	Stats = new CharacterStats(file);
	e->AddComponent(Stats);

	file.close();
}


Character::~Character()
{
}

	
void Character::Load()
{
	// Save();

	RB = Entity->GetComponent<RigidBody>(); 
	MoveDirection = Entity->Transform.WSForward();
}


void Character::Update()
{
	glm::quat targetRotation = glm::RotationBetweenVectors(Entity->Transform.Forward, MoveDirection);
	Entity->Transform.Rotation = glm::mix(
		(glm::dot(targetRotation, Entity->Transform.Rotation) > 0) ? Entity->Transform.Rotation : -Entity->Transform.Rotation,
		targetRotation,
		glm::min(Time::Delta * 10.0f, 1.0f));

	if (MoveMode != Input::MovementMode::Idle)
	{
		// Apply Movement
		auto targetVelocity = MoveDirection * ((MoveMode == Input::MovementMode::Run) ? RunSpeed : (MoveMode == Input::MovementMode::Walk) ? WalkSpeed : DodgeSpeed);
		auto velocity = RB->GetLinearVelocity();
		auto velocityChange = (targetVelocity - velocity);
		velocityChange.y = 0;
		velocityChange.x = glm::clamp(velocityChange.x, -MAX_VELOCITY_DELTA, MAX_VELOCITY_DELTA);
		velocityChange.z = glm::clamp(velocityChange.z, -MAX_VELOCITY_DELTA, MAX_VELOCITY_DELTA);
		RB->ApplyCenterForce(velocityChange * (1.0f / Time::Delta) * 5.0f);

		RB->GetBody()->setAngularVelocity(btVector3(0, 0, 0));
	}
	else
	{
		// Apply Movement
		auto targetVelocity = glm::vec3(0.0f);
		auto velocity = RB->GetLinearVelocity();
		auto velocityChange = (targetVelocity - velocity);
		velocityChange.x = glm::clamp(velocityChange.x, -MAX_VELOCITY_DELTA, MAX_VELOCITY_DELTA);
		velocityChange.z = glm::clamp(velocityChange.z, -MAX_VELOCITY_DELTA, MAX_VELOCITY_DELTA);
		velocityChange.y = 0;
		RB->ApplyCenterForce(velocityChange * (1.0f / Time::Delta) * 5.0f);

		RB->GetBody()->setAngularVelocity(btVector3(0, 0, 0));
	}

	if (!IsControlEnabled())
	{
		InputLockElapsed += Time::Delta;
		if (InputLockElapsed > InputLockDuration)
			InputLockDuration = -1.0f;
	}
}


void Character::Save()
{
	if (Settings::Misc::VerboseLogging)
		Debug::Log("Saving character: " + Name);

	std::ofstream file("Resources/Characters/" + Name);
	if (!file)
	{
		Debug::Log("Could not save character: " + Name);
		return;
	}

	file << std::to_string(Height) << std::endl;
	file << std::to_string(Entity->Transform.Scale.x) << std::endl;
	SA->Save(file);
	Stats->Save(file);
	file.close();
}



void Character::Move(Input::MovementMode mode, const glm::vec2& moveDirection)
{
	if (MoveMode != mode)
	{
		if (MoveMode == Input::MovementMode::Idle || MoveMode == Input::MovementMode::Dodge)
		{
			if (mode == Input::MovementMode::Run)
			{
				SA->PlayAnimation("IdleToRun", false);
			}
			else // if (mode == Input::MovementMode::Walk)
			{
				SA->PlayAnimation("IdleToWalk", false);
			}
		}
		else if (MoveMode == Input::MovementMode::Walk)
		{
			if (mode == Input::MovementMode::Idle)
			{
				SA->PlayAnimation("WalkToIdle", false);
			}
			else // if (mode == Input::MovementMode::Run)
			{
				SA->PlayAnimation("Run", true);
			}
		}
		else if (MoveMode == Input::MovementMode::Run)
		{
			if (mode == Input::MovementMode::Idle)
			{
				SA->PlayAnimation("RunToIdle", false);
			}
			else // if (mode == Input::MovementMode::Walk)
			{
				SA->PlayAnimation("Walk", true);
			}
		}
		MoveMode = mode;
	}
	MoveDirection = glm::vec3(moveDirection.x, 0.0f, moveDirection.y);
}



void Character::StopMoving()
{
	if (MoveMode != Input::MovementMode::Idle)
	{
		//RB->SetLinearVelocity(glm::vec3(0, 0, 0) + glm::vec3(0.0f, RB->GetLinearVelocity().y, 0.0f));
		if (MoveMode == Input::MovementMode::Run)
			SA->PlayAnimation("RunToIdle", false);
		else if (MoveMode == Input::MovementMode::Walk)
			SA->PlayAnimation("WalkToIdle", false);
		else
			SA->PlayAnimation("Idle", true);
		MoveMode = Input::MovementMode::Idle;
	}
}



void Character::Dodge(const glm::vec2& direction)
{
	if (MoveMode != Input::MovementMode::Dodge)
	{
		MoveMode = Input::MovementMode::Dodge;
		SA->PlayAnimation("Dodge", false);
		MoveDirection = glm::vec3(direction.x, 0.0f, direction.y);

		LockInput(0.5f);
	}
}



void Character::PrimaryAttack(const glm::vec2& direction)
{
	MoveMode = Input::MovementMode::Idle;
	SA->PlayAnimation("PrimaryAttack", false);
	MoveDirection = glm::vec3(direction.x, 0.0f, direction.y);
	LockInput(SA->GetAnimationTime("PrimaryAttack"));
}



void Character::SecondaryAttack(const glm::vec2& direction)
{
	MoveMode = Input::MovementMode::Idle;
	SA->PlayAnimation("SecondaryAttack", false);
	MoveDirection = glm::vec3(direction.x, 0.0f, direction.y);
	LockInput(SA->GetAnimationTime("SecondaryAttack"));
}


	
void Character::LockInput(float duration)
{
	InputLockDuration = duration;
	InputLockElapsed = 0.0f;
}



bool Character::IsControlEnabled()
{
	return InputLockDuration < -0.5f;
}