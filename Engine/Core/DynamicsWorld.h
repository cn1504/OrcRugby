#pragma once
#include "Core.h"
#include "RigidBody.h"
#include "SoftBody.h"

class btThreadSupportInterface;

namespace Core
{
	class Window;
	class RigidBody;
	class SoftBody;

	class DynamicsWorld
	{
	private:		
		btSoftBodyRigidBodyCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		btSoftRigidDynamicsWorld* dynamicsWorld;
		
		btThreadSupportInterface*		m_threadSupportCollision;
		btThreadSupportInterface*		m_threadSupportSolver;

		btAlignedObjectArray<btCollisionShape*> collisionShapes;
		std::vector<btTypedConstraint*> Constraints;

	public:
		std::unordered_map<btCollisionObject*, FreeBody*> BodyMap;

		btSoftBodyWorldInfo	softBodyWorldInfo;

		DynamicsWorld();
		~DynamicsWorld();

		void AddBody(btCollisionObject* body);
		void AddBody(RigidBody* body);
		void AddBody(SoftBody* body);

		void RemoveBody(RigidBody* body);
		void RemoveBody(SoftBody* body);

		void AddConstraint(btTypedConstraint* constraint);
		void RemoveConstraint(btTypedConstraint* constraint);
		
		void Update();

		void GetAllEntitiesWithinBroadphase(btCollisionShape& shape, btTransform& t, std::vector<Entity*> &entities);
		bool RayCastHitPoint(const glm::vec3& start, const glm::vec3& end, glm::vec3& hitpoint, Entity** hitObject);


		void myTickCallback(const btDynamicsWorld *world, btScalar timeStep);
	};

}