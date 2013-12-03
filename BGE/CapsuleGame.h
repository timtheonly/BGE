#pragma once
#include "game.h"
#include "PhysicsFactory.h"
#include "GLDebugDrawer.h"
#include "PhysicsController.h"

namespace BGE
{
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
			std::shared_ptr<PhysicsController> caps;
		
			// The world.
			std::shared_ptr<PhysicsFactory> physicsFactory;
			btDiscreteDynamicsWorld * dynamicsWorld;
			GLDebugDrawer * debugDraw;
	};

}