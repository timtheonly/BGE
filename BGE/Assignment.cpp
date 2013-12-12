#include "Assignment.h"
#include "Content.h"
#include <btBulletDynamicsCommon.h>
using namespace BGE;

Assignment::Assignment(void)
{
	effectYSpeed = 0.5f;
	effectTheta = 0;
	physicsFactory = NULL;
	dynamicsWorld = NULL;
	broadphase = NULL;
	dispatcher = NULL;
	solver = NULL;
}


Assignment::~Assignment(void)
{
}

bool Assignment::Initialise()
{
	// Set up the collision configuration and dispatcher
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
 
    // The world.
	btVector3 worldMin(-1000,-1000,-1000);
	btVector3 worldMax(1000,1000,1000);
	broadphase = new btAxisSweep3(worldMin,worldMax);
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,-9,0));


	physicsFactory = make_shared<PhysicsFactory>(dynamicsWorld);

	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();

	physicsFactory->CreateFromModel("hat",glm::vec3(1),glm::quat(),glm::vec3(1));


	fullscreen = false;
	width = 800;
	height = 600;

	physicsFactory->CreateStairs(glm::vec3(-1,2,0));
	physicsFactory->CreateRagDoll(glm::vec3(-1,10,0));
	

	if (!Game::Initialise()) {
		return false;
	}
	return true;
}

void Assignment::Update(float gameTime)
{	
	dynamicsWorld->stepSimulation(gameTime,1);
	Game::Update(gameTime);
}

void Assignment::Cleanup()
{
	Game::Cleanup();
}