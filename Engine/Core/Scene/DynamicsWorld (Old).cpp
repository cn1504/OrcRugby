#include "DynamicsWorld.h"
#include <thread>


#include <Bullet/BulletMultiThreaded/SpuGatheringCollisionDispatcher.h>
#include <Bullet/BulletMultiThreaded/PlatformDefinitions.h>
#include <Bullet/BulletMultiThreaded/Win32ThreadSupport.h>
#include <Bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h>
#include <Bullet/BulletMultiThreaded/btParallelConstraintSolver.h>
#include <Bullet/BulletMultiThreaded/SequentialThreadSupport.h>
#include <Bullet/BulletCollision/CollisionDispatch/btSimulationIslandManager.h>

//#define SEQUENTIAL
btThreadSupportInterface* createSolverThreadSupport(int maxNumThreads)
{
	#ifdef SEQUENTIAL
	SequentialThreadSupport::SequentialThreadConstructionInfo tci("solverThreads", SolverThreadFunc, SolverlsMemoryFunc);
	SequentialThreadSupport* threadSupport = new SequentialThreadSupport(tci);
	threadSupport->startSPU();
	#else
	Win32ThreadSupport::Win32ThreadConstructionInfo threadConstructionInfo("solverThreads", SolverThreadFunc, SolverlsMemoryFunc, maxNumThreads);
	Win32ThreadSupport* threadSupport = new Win32ThreadSupport(threadConstructionInfo);
	threadSupport->startSPU();
	#endif
	return threadSupport;
}


namespace Core
{
	
	DynamicsWorld::DynamicsWorld()
	{
		m_threadSupportSolver = 0;
		m_threadSupportCollision = 0;
		int maxNumOutstandingTasks = std::thread::hardware_concurrency();
		if (maxNumOutstandingTasks == 0)
			maxNumOutstandingTasks = 4;

		Debug::Log("Hardware Concurrency: " + std::to_string(std::thread::hardware_concurrency()));

		#ifdef SEQUENTIAL
		SequentialThreadSupport::SequentialThreadConstructionInfo colCI("collision", processCollisionTask, createCollisionLocalStoreMemory);
		m_threadSupportCollision = new SequentialThreadSupport(colCI);
		#else
		m_threadSupportCollision = new Win32ThreadSupport(Win32ThreadSupport::Win32ThreadConstructionInfo(
			"collision",
			processCollisionTask,
			createCollisionLocalStoreMemory,
			maxNumOutstandingTasks));
		#endif

		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration. 
		btDefaultCollisionConstructionInfo cci;
		cci.m_defaultMaxPersistentManifoldPoolSize = 32768;
		collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration(cci);


		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		//dispatcher = new btCollisionDispatcher(collisionConfiguration);
		dispatcher = new SpuGatheringCollisionDispatcher(m_threadSupportCollision, maxNumOutstandingTasks, collisionConfiguration);
		softBodyWorldInfo.m_dispatcher = dispatcher;

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep. 
		overlappingPairCache = new btDbvtBroadphase(); 
		softBodyWorldInfo.m_broadphase = overlappingPairCache;
		
		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded) 
		//solver = new btSequentialImpulseConstraintSolver;
		#ifdef SEQUENTIAL
		solver = new btSequentialImpulseConstraintSolver();
		#else
		m_threadSupportSolver = createSolverThreadSupport(maxNumOutstandingTasks);
		solver = new btParallelConstraintSolver(m_threadSupportSolver);

		//this solver requires the contacts to be in a contiguous pool, so avoid dynamic allocation
		dispatcher->setDispatcherFlags(btCollisionDispatcher::CD_DISABLE_CONTACTPOOL_DYNAMIC_ALLOCATION);
		#endif

		dynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration); // , softBodySolver);

		dynamicsWorld->getSimulationIslandManager()->setSplitIslands(false);
		dynamicsWorld->getSolverInfo().m_numIterations = 4;
		dynamicsWorld->getSolverInfo().m_solverMode = SOLVER_SIMD + SOLVER_USE_WARMSTARTING;//+SOLVER_RANDMIZE_ORDER;

		dynamicsWorld->getDispatchInfo().m_enableSPU = true;

		dynamicsWorld->setGravity(btVector3(0,-9.8f,0));

		softBodyWorldInfo.m_gravity.setValue(0, -9.8f, 0);
		softBodyWorldInfo.m_sparsesdf.Initialize();
	}


	DynamicsWorld::~DynamicsWorld()
	{
		for (auto c : Constraints)
		{
			dynamicsWorld->removeConstraint(c);
			delete c;
		}
		Constraints.clear();

		//remove the rigidbodies from the dynamics world and delete them 
		for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0 ; i--) 
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i]; 
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{ 
				delete body->getMotionState(); 
			} 
			dynamicsWorld->removeCollisionObject( obj ); 
			delete obj;
		}

		//delete collision shapes
		for (int j = 0; j < collisionShapes.size(); j++)
		{ 
			btCollisionShape* shape = collisionShapes[j]; 
			collisionShapes[j] = 0; 
			delete shape; 
		} 

		delete dynamicsWorld;
		delete solver;

		if (m_threadSupportSolver)
		{
			delete m_threadSupportSolver;
		}

		delete overlappingPairCache;
		delete dispatcher;

		deleteCollisionLocalStoreMemory();
		if (m_threadSupportCollision)
		{
			delete m_threadSupportCollision;
		}

		delete collisionConfiguration;
	}


	void DynamicsWorld::AddBody(RigidBody* body)
	{
		dynamicsWorld->addRigidBody(body->GetBody());
		collisionShapes.push_back(body->GetBody()->getCollisionShape());
		BodyMap[body->GetBody()] = body;
	}


	void DynamicsWorld::AddBody(SoftBody* psb)
	{
		dynamicsWorld->addSoftBody(psb->GetBody());
		BodyMap[psb->GetBody()] = psb;
	}

	
	void DynamicsWorld::RemoveBody(RigidBody* body)
	{
		if (BodyMap.count(body->GetBody()) > 0)
		{
			dynamicsWorld->removeRigidBody(body->GetBody());
			BodyMap.erase(body->GetBody());
		}
	}
	void DynamicsWorld::RemoveBody(SoftBody* body)
	{
		if (BodyMap.count(body->GetBody()) > 0)
		{
			dynamicsWorld->removeSoftBody(body->GetBody());
			BodyMap.erase(body->GetBody());
		}
	}


	void DynamicsWorld::AddConstraint(btTypedConstraint* constraint)
	{		
		dynamicsWorld->addConstraint(constraint);
		Constraints.push_back(constraint);
	}
	void DynamicsWorld::RemoveConstraint(btTypedConstraint* constraint)
	{
		if (Constraints.size() > 0)
		{
			dynamicsWorld->removeConstraint(constraint);

			int i = 0;
			while (i < Constraints.size())
			{
				if (Constraints[i] == constraint)
					break;
				i++;
			}

			Constraints[i] = Constraints.back();
			Constraints.resize(Constraints.size() - 1);
		}
	}


	void DynamicsWorld::Update()
	{
		dynamicsWorld->stepSimulation(Time::Delta, 10);

		for (auto fb : BodyMap)
		{
			auto rb = fb.second->Entity->GetComponent<RigidBody>();
			if (rb != nullptr)
			{
				btRigidBody* body = rb->GetBody();

				if (body && body->getMotionState())
				{
					btTransform trans;
					body->getMotionState()->getWorldTransform(trans);

					auto p = trans.getOrigin();
					auto r = trans.getRotation();

					Transform t;
					t.Position = glm::vec3(p.getX(), p.getY(), p.getZ());
					t.Rotation = glm::quat(r.getW(), r.getX(), r.getY(), r.getZ());

					rb->Entity->Transform.Position = glm::vec3(t.ToMatrix() * glm::vec4(rb->GetOffset(), 1.0f));
					if (rb->Entity->GetComponent<Character>() == nullptr)
						rb->Entity->Transform.Rotation = t.Rotation;
				}
			}

			auto sb = fb.second->Entity->GetComponent<SoftBody>();
			if (sb != nullptr)
			{
				sb->RebuildMesh();
			}
		}

		softBodyWorldInfo.m_sparsesdf.GarbageCollect();
	}


	struct BroadphaseAabbCallback : public btBroadphaseAabbCallback {
		btAlignedObjectArray < btCollisionObject* >& m_collisionObjectArray;
		short int m_collisionFilterGroup, m_collisionFilterMask;// Optional
		BroadphaseAabbCallback(btAlignedObjectArray < btCollisionObject* >& collisionObjectArray, short int collisionGroup = btBroadphaseProxy::DefaultFilter, short int collisionMask = btBroadphaseProxy::AllFilter) :
			m_collisionObjectArray(collisionObjectArray), m_collisionFilterGroup(collisionGroup), m_collisionFilterMask(collisionMask)
		{
			m_collisionObjectArray.resize(0);
		}
		// Optional:
		SIMD_FORCE_INLINE bool needsCollision(const btBroadphaseProxy* proxy) const {
			bool collides = (proxy->m_collisionFilterGroup & m_collisionFilterMask) != 0;
			collides = collides && (m_collisionFilterGroup & proxy->m_collisionFilterMask);
			return collides;
		}

		virtual bool process(const btBroadphaseProxy *proxy) {
			// Basically this is the only method to implement in btBroadphaseAabbCallback.
			// If we don't need any filtering/excluding objects, we can just write:
			// m_collisionObjectArray.push_back((btCollisionObject *)proxy->m_clientObject);return true;
			// and we're done

			if (needsCollision(proxy)) m_collisionObjectArray.push_back((btCollisionObject *)proxy->m_clientObject);
			return true;
		}
	};

	void DynamicsWorld::GetAllEntitiesWithinBroadphase(btCollisionShape& shape, btTransform& t, std::vector<Entity*> &entities)
	{
		// Get a list of entities that could be intersecting with the shape object using bullet's broadphase test
		btVector3 min, max;
		shape.getAabb(t, min, max);

		btAlignedObjectArray < btCollisionObject* > collisionObjectArray;
		BroadphaseAabbCallback callback(collisionObjectArray);
		dynamicsWorld->getBroadphase()->aabbTest(min, max, callback);

		
		for (int i = 0; i < collisionObjectArray.size(); i++)
		{
			entities.push_back(BodyMap[collisionObjectArray[i]]->Entity);
		}
	}

	
	void DynamicsWorld::AddBody(btCollisionObject* body)
	{
		dynamicsWorld->addCollisionObject(body);
	}

	
	bool DynamicsWorld::RayCastHitPoint(const glm::vec3& start, const glm::vec3& end, glm::vec3& hitpoint, Entity** hitObject)
	{
		auto Start = btVector3(start.x, start.y, start.z);
		auto End = btVector3(end.x, end.y, end.z);
		btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);

		// Perform raycast
		dynamicsWorld->rayTest(Start, End, RayCallback);

		if (RayCallback.hasHit()) {
			hitpoint = glm::vec3(RayCallback.m_hitPointWorld.getX(), RayCallback.m_hitPointWorld.getY(), RayCallback.m_hitPointWorld.getZ());
			auto fb = (FreeBody*)(RayCallback.m_collisionObject->getUserPointer());
			*hitObject = fb->Entity;
		}
		return RayCallback.hasHit();
	}

}