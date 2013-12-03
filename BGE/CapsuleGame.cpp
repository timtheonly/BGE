#include "CapsuleGame.h"
#include "Capsule.h"
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
    dynamicsWorld->setGravity(btVector3(0,0,0));
	debugDraw = new GLDebugDrawer();
	dynamicsWorld->setDebugDrawer(debugDraw);

	physicsFactory = make_shared<PhysicsFactory>(dynamicsWorld);

	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();

	caps =  physicsFactory->CreateCapsule(2.5f,6.0f,glm::vec3(1,0,-1),glm::quat());

	shared_ptr<GameComponent> capsule = make_shared<Capsule>(2.5f,6.0f);
	capsule->position = glm::vec3(1,0,-1);
	capsule->orientation = glm::quat();
	Attach(capsule);
	return Game::Initialise();
}

CapsuleGame::~CapsuleGame(void)
{

}

void CapsuleGame::Update(float gameTime)
{
	dynamicsWorld->stepSimulation(gameTime,100);
	debugDraw->drawCapsule(2.5f,3.0f,1,btTransform(GLToBtQuat(caps->orientation), GLToBtVector(caps->position)),btVector3(1,0,0));
	dynamicsWorld->debugDrawWorld();
	Game::Update(gameTime);
}

void CapsuleGame::Cleanup()
{
	Game::Cleanup();
}