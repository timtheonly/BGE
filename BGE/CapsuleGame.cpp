#include "CapsuleGame.h"
#include "Utils.h"
#include <btBulletDynamicsCommon.h>

using namespace BGE;

CapsuleGame::CapsuleGame(void)
{
	physicsFactory = NULL;
	dynamicsWorld = NULL;
	broadphase = NULL;
	dispatcher = NULL;
	solver = NULL;
	fullscreen = false;
	width = 800;
	height = 600;
}
bool CapsuleGame::Initialise() 
{
	riftEnabled = false;
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

	caps =  physicsFactory->CreateCapsule(0.5f,0.5f,glm::vec3(1,0,-1),glm::quat());
	physicsFactory->CreateRagDoll(glm::vec3(5,3,1));

	return Game::Initialise();
}

CapsuleGame::~CapsuleGame(void)
{

}

void CapsuleGame::Update(float gameTime)
{
	dynamicsWorld->stepSimulation(gameTime,100);
	Game::Update(gameTime);
}

void CapsuleGame::Cleanup()
{
	Game::Cleanup();
}