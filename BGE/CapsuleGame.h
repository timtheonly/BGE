#pragma once
#include "game.h"
#include "PhysicsFactory.h"

using namespace BGE;

class CapsuleGame :
	public Game
{
	private:
		btBroadphaseInterface* broadphase;
 
		// Set up the collision configuration and dispatcher
		btDefaultCollisionConfiguration * collisionConfiguration;
		btCollisionDispatcher * dispatcher;
 
		// The actual physics solver
		btSequentialImpulseConstraintSolver * solver;
	public:
		CapsuleGame(void);
		~CapsuleGame(void);
		bool Initialise();
		void Update(float timeDelta);
		void Cleanup();
		void CreateWall();
		
		// The world.
		std::shared_ptr<PhysicsFactory> physicsFactory;
		btDiscreteDynamicsWorld * dynamicsWorld;
};

