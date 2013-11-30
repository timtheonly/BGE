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

	hat = make_shared<GameComponent>();
	hat->Attach(Content::LoadModel("hat"));
	hat->position = glm::vec3(0,1,-1);
	hat->diffuse= glm::vec3(0.0f,0.0f,1.0f);
	hat->Attach(physicsFactory->CreateFromModel("hat",hat->position,glm::quat(),glm::vec3(1)));
	Attach(hat);

	fullscreen = false;
	width = 800;
	height = 600;

	physicsFactory->CreateCylinder(5.0f,4.0f,glm::vec3(0,0,1),glm::quat());
	hatFountain = make_shared<ExpansionEffect>();
	hatFountain->position =hat->position;
	hatFountain->diffuse = glm::vec3(1,0,1);
	Attach(hatFountain);

	if (!Game::Initialise()) {
		return false;
	}
	return true;
}

void Assignment::Update(float gameTime)
{

	hat->Yaw(0.5f);
	/*
	if(hatFountain->position.y > 5)
	{
		hatFountain->position.y = 5.0f;
		effectYSpeed = -effectYSpeed;
	}

	if(hatFountain->position.y < 0)
	{
		hatFountain->position.y = 0.0f;
		effectYSpeed = -effectYSpeed;
	}*/
	
	dynamicsWorld->stepSimulation(gameTime,100);
	
	/*
	effectTheta += gameTime;
	if(effectTheta >= glm::pi<float>() *2)
	{
		effectTheta = 0;
	}
	
	hatFountain->position.x = glm::cos(effectTheta);
	hatFountain->position.z = glm::sin(effectTheta);
	hatFountain->position.y += effectYSpeed * gameTime;*/
	Game::Update(gameTime);
}

void Assignment::Cleanup()
{
	Game::Cleanup();
}