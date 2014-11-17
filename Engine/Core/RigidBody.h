#pragma once
#include "Core.h"
#include "FreeBody.h"

namespace Core
{
	class DynamicsWorld;
	class Material;


	class RigidBody
		: public FreeBody
	{		
	public:
		enum class Type { DYNAMIC, KINEMATIC, STATIC };

		RigidBody(DynamicsWorld* world, Material* material, btCollisionShape* shape, const glm::vec3& positionOffset = glm::vec3(), float mass = 0.0f, Type type = Type::STATIC);
		virtual ~RigidBody();
		virtual void Load();

		btRigidBody* GetBody();
		
		glm::vec3 GetOffset();
		float GetMass();
		glm::vec3 GetLinearVelocity();
		void SetLinearVelocity(const glm::vec3& v);
		void SetGravity(const glm::vec3& g);		
		void ApplyForce(const glm::vec3& force, const glm::vec3& location);
		void ApplyCenterForce(glm::vec3 force);
		void ApplyImpulse(const glm::vec3& force, const glm::vec3& location);
		void ApplyCenterImpulse(glm::vec3 force);
		void ApplyTorque(const glm::vec3& torque);
		void ApplyTorqueImpulse(const glm::vec3& torque);

		void SetFriction(float f);
		void ApplyFriction();
		void DisableFriction();

		void SetAxisFactors(const glm::vec3& linear, const glm::vec3& angular);
		
	protected:
		Type type;
		btCollisionShape* Shape;
		glm::vec3 PositionOffset;
		btScalar Mass;

		btRigidBody* body;

		float Friction;
	};

}