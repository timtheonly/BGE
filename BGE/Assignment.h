#pragma once
#include "Game.h"
#include "GameComponent.h"
#include "PhysicsController.h"
#include "PhysicsFactory.h"
#include "ExpansionEffect.h"

using namespace std;

namespace BGE
{
	class Assignment : public  Game
	{
	private:
		btBroadphaseInterface* broadphase;
 
		// Set up the collision configuration and dispatcher
		btDefaultCollisionConfiguration * collisionConfiguration;
		btCollisionDispatcher * dispatcher;
 
		// The actual physics solver
		btSequentialImpulseConstraintSolver * solver;
	public:
		float effectTheta;
		float effectYSpeed;
		shared_ptr<ExpansionEffect> hatFountain;
		shared_ptr<GameComponent> hat;
		Assignment(void);
		~Assignment(void);
		bool Initialise();
		void Update(float timeDelta);
			
		// The world.
		std::shared_ptr<PhysicsFactory> physicsFactory;
		btDiscreteDynamicsWorld * dynamicsWorld;
	};

}