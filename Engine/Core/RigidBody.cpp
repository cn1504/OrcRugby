#include "RigidBody.h"
#include "DynamicsWorld.h"
#include "Material.h"

namespace Core
{

	RigidBody::RigidBody(DynamicsWorld* world, Material* material, btCollisionShape* shape, const glm::vec3& positionOffset, float mass, Type type)
		: FreeBody(world, material)
	{
		this->type = type;
		Shape = shape;

		PositionOffset = positionOffset;
		Mass = mass;
	}


	RigidBody::~RigidBody()
	{
		world->RemoveBody(this);
		if (Shape != nullptr)
			delete Shape;
	}


	void RigidBody::Load()
	{
		btCollisionShape* colShape = Shape;

		btTransform startTransform;
		startTransform.setIdentity();

		startTransform.setOrigin(btVector3(Entity->Transform.Position.x, Entity->Transform.Position.y, Entity->Transform.Position.z));
		startTransform.setRotation(btQuaternion(Entity->Transform.Rotation.x, Entity->Transform.Rotation.y, Entity->Transform.Rotation.z, Entity->Transform.Rotation.w));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btMotionState* myMotionState = new btDefaultMotionState(startTransform);

		btVector3 localInertia(0, 0, 0);
		if (type == Type::DYNAMIC)
		{			
			colShape->calculateLocalInertia(Mass, localInertia);
		}
		else // Static or Kinematic
		{
			Mass = btScalar(0.0);
		}

		btRigidBody::btRigidBodyConstructionInfo rbInfo(Mass, myMotionState, colShape, localInertia);
		body = new btRigidBody(rbInfo);
		body->setUserPointer(this);

		if (type == Type::KINEMATIC)
		{
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			body->setActivationState(DISABLE_DEACTIVATION);
		}

		world->AddBody(this);

		Friction = body->getFriction();
	}


	btRigidBody* RigidBody::GetBody()
	{
		return body;
	}


	float RigidBody::GetMass()
	{
		return 1.0f / body->getInvMass();
	}


	glm::vec3 RigidBody::GetLinearVelocity()
	{
		auto v = body->getLinearVelocity();
		return glm::vec3(v.getX(), v.getY(), v.getZ());
	}
	
	void RigidBody::SetLinearVelocity(const glm::vec3& v)
	{
		body->setLinearVelocity(btVector3(v.x, v.y, v.z));
		body->activate();
	}


	void RigidBody::SetGravity(const glm::vec3& g)
	{
		body->setGravity(btVector3(g.x, g.y, g.z));
		body->activate();
	}


	void RigidBody::ApplyForce(const glm::vec3& force, const glm::vec3& location)
	{
		body->applyForce(btVector3(force.x, force.y, force.z), btVector3(location.x, location.y, location.z));
		body->activate();
	}
	void RigidBody::ApplyCenterForce(glm::vec3 force)
	{
		body->applyCentralForce(btVector3(force.x, force.y, force.z));
		body->activate();
	}

	void RigidBody::ApplyImpulse(const glm::vec3& force, const glm::vec3& location)
	{
		body->applyImpulse(btVector3(force.x, force.y, force.z), btVector3(location.x, location.y, location.z));
		body->activate();
	}
	void RigidBody::ApplyCenterImpulse(glm::vec3 force)
	{
		body->applyCentralImpulse(btVector3(force.x, force.y, force.z));
		body->activate();
	}


	void RigidBody::ApplyTorque(const glm::vec3& torque)
	{
		body->applyTorque(btVector3(torque.x, torque.y, torque.z));
		body->activate();
	}
	void RigidBody::ApplyTorqueImpulse(const glm::vec3& torque)
	{
		body->applyTorqueImpulse(btVector3(torque.x, torque.y, torque.z));
		body->activate();
	}



	glm::vec3 RigidBody::GetOffset()
	{
		return PositionOffset;
	}


	void RigidBody::SetFriction(float f)
	{
		Friction = f;
		ApplyFriction();
	}

	void RigidBody::ApplyFriction()
	{
		body->setFriction(Friction);
	}

	void RigidBody::DisableFriction()
	{
		body->setFriction(0);
	}
	

	void RigidBody::SetAxisFactors(const glm::vec3& linear, const glm::vec3& angular)
	{
		world->RemoveBody(this);

		body->setLinearFactor(btVector3(linear.x, linear.y, linear.z));
		body->setAngularFactor(btVector3(angular.x, angular.y, angular.z));

		world->AddBody(this);
	}
		
}