#include "PhysicsFactory.h"
#include "Game.h"
#include "Sphere.h"
#include "Box.h"
#include "Cylinder.h"
#include "Ground.h"
#include "Content.h"
#include "PhysicsCamera.h"
#include "Model.h"
#include "dirent.h"
#include "Utils.h"
using namespace BGE;

PhysicsFactory::PhysicsFactory(btDiscreteDynamicsWorld * dynamicsWorld)
{
	this->dynamicsWorld = dynamicsWorld;
}


PhysicsFactory::~PhysicsFactory(void)
{
}

void PhysicsFactory::CreateWall(glm::vec3 startAt, float width, float height, float blockWidth, float blockHeight, float blockDepth)
{
	float z = startAt.z;
	float gap = 1;

	for (int w = 0 ; w < width ; w ++)
	{
		for (int h = 0 ; h < height ; h ++)	
		{
			float x = startAt.x + ((blockWidth + 2) * w);
			float y = ((blockHeight + gap) / 2.0f) + ((blockHeight + gap) * h);
			CreateBox(blockWidth, blockHeight, blockDepth, glm::vec3(x, y, z), glm::quat());
		}
	}
}


shared_ptr<PhysicsController> PhysicsFactory::CreateFromModel(string name, glm::vec3 pos, glm::quat quat, glm::vec3 scale)
{
	shared_ptr<GameComponent> component = make_shared<GameComponent>();
	component->tag = name;
	component->scale = scale;
	Game::Instance()->Attach(component);
	shared_ptr<Model> model = Content::LoadModel(name);
	model->Initialise();

	std::vector<glm::vec3>::iterator it = model->vertices.begin(); 	
	btConvexHullShape * tetraShape = new btConvexHullShape();

	while (it != model->vertices.end())
	{
		glm::vec4 point = glm::vec4(* it, 0) * glm::scale(glm::mat4(1), scale);
		tetraShape->addPoint(GLToBtVector(glm::vec3(point)));
		it ++;
	}
	
	btScalar mass = 1;
	btVector3 inertia(0,0,0);
	
	tetraShape->calculateLocalInertia(mass,inertia);
	btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(GLToBtQuat(quat)
		,GLToBtVector(pos)));	
	
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,motionState, tetraShape, inertia);
	btRigidBody * body = new btRigidBody(rigidBodyCI);
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	dynamicsWorld->addRigidBody(body);

	shared_ptr<PhysicsController> controller =make_shared<PhysicsController>(tetraShape, body, motionState);	
	body->setUserPointer(controller.get());
	component->Attach(controller);
	
	controller->tag = "Model";	
	return controller;
}

shared_ptr<PhysicsController> PhysicsFactory::CreateSphere(float radius, glm::vec3 pos, glm::quat quat)
{
	shared_ptr<GameComponent> sphere (new Sphere(radius));
	Game::Instance()->Attach(sphere);

	btDefaultMotionState * sphereMotionState = new btDefaultMotionState(btTransform(GLToBtQuat(quat)
		,GLToBtVector(pos)));	

	btScalar mass = 1;
	btVector3 sphereInertia(0,0,0);
	btCollisionShape * sphereShape = new btSphereShape(radius);

	sphereShape->calculateLocalInertia(mass,sphereInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,sphereMotionState, sphereShape, sphereInertia);
	btRigidBody * body = new btRigidBody(fallRigidBodyCI);
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	dynamicsWorld->addRigidBody(body);

	shared_ptr<PhysicsController> sphereController (new PhysicsController(sphereShape, body, sphereMotionState));	
	body->setUserPointer(sphereController.get());
	sphere->Attach(sphereController);
	sphereController->tag = "Sphere";	
	return sphereController;
}


shared_ptr<PhysicsController> PhysicsFactory::CreateCapsule(float radius, float height, glm::vec3 pos, glm::quat quat)
{
	//make bullet shape
	btCollisionShape *capShape = new btCapsuleShape(btScalar(radius),btScalar(height));
	btScalar mass = 1;
	btVector3 capInertia(0,0,0);
	capShape->calculateLocalInertia(mass,capInertia);

	//combine multiple models to create a capsule
	shared_ptr<Cylinder>  capsule = make_shared<Cylinder>(radius, height);
	capsule->position = pos;
	
	//add first cap
	shared_ptr<Sphere> sphere1 = make_shared<Sphere>(radius);
	sphere1->worldMode = GameComponent::from_self_with_parent;
	sphere1->position = glm::vec3(0, +(height-radius)-0.25f,0);
	capsule->Attach(sphere1);

	//add second cap
	shared_ptr<Sphere> sphere2 = make_shared<Sphere>(radius);
	sphere2->worldMode = GameComponent::from_self_with_parent;
	sphere2->position = glm::vec3(0, -(height-radius)+0.25f,0);
	capsule->Attach(sphere2);
	Game::Instance()->Attach(capsule);
	

	//create a rigid body
	btDefaultMotionState *capMotionState = new btDefaultMotionState(btTransform(GLToBtQuat(quat), GLToBtVector(pos)));
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, capMotionState, capShape,capInertia);
	btRigidBody * body = new btRigidBody(fallRigidBodyCI);
	dynamicsWorld->addRigidBody(body);
	
	//make a physicsController and attach it
	shared_ptr<PhysicsController> capControllor = make_shared<PhysicsController>(PhysicsController(capShape, body, capMotionState));
	capControllor->tag = "Capsule";
	body->setUserPointer(capControllor.get());
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	capsule->Attach(capControllor);

	return capControllor;
}


shared_ptr<PhysicsController> PhysicsFactory::CreateBox(float width, float height, float depth, glm::vec3 pos, glm::quat quat)
{
	// Create the shape
	btCollisionShape * boxShape = new btBoxShape(btVector3(width, height, depth) * 0.50);
	btScalar mass = 1;
	btVector3 boxInertia(0,0,0);
	boxShape->calculateLocalInertia(mass,boxInertia);

	// This is a container for the box model
	shared_ptr<Box> box = make_shared<Box>(width, height, depth);
	box->worldMode = GameComponent::from_child;
	box->position = pos;
	Game::Instance()->Attach(box);

	// Create the rigid body
	btDefaultMotionState * boxMotionState = new btDefaultMotionState(btTransform(GLToBtQuat(quat)
		,GLToBtVector(pos)));			
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,  boxMotionState, boxShape, boxInertia);
	btRigidBody * body = new btRigidBody(fallRigidBodyCI);
	body->setFriction(567);
	dynamicsWorld->addRigidBody(body);

	// Create the physics component and add it to the box
	shared_ptr<PhysicsController> boxController = make_shared<PhysicsController>(PhysicsController(boxShape, body, boxMotionState));
	boxController->tag = "Box";
	body->setUserPointer(boxController.get());
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	box->Attach(boxController);

	return boxController;
}
 shared_ptr<PhysicsController> PhysicsFactory::CreateKinematicCylinder(float radius, float height, glm::vec3 pos, glm::quat quat)
 {
 
	// Create the shape
	btCollisionShape * shape = new btCylinderShape(btVector3(radius, height * 0.5f, radius));
	btScalar mass = 0;
	btVector3 inertia(0,0,0);
	shape->calculateLocalInertia(mass,inertia);

	// This is a container for the box model
	shared_ptr<GameComponent> cyl = make_shared<GameComponent>(Cylinder(radius, height));
	cyl->position = pos;
	Game::Instance()->Attach(cyl);

	// Create the rigid body
	btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(GLToBtQuat(quat)
		,GLToBtVector(pos)));			
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,  motionState, shape, inertia);
	btRigidBody * body = new btRigidBody(rigidBodyCI);
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	dynamicsWorld->addRigidBody(body);

	// Create the physics component and add it to the box
	shared_ptr<PhysicsController> component = make_shared<PhysicsController>(PhysicsController(shape, body, motionState));
	body->setUserPointer(component.get());
	component->tag = "Cylinder";
	cyl->Attach(component);

	return component;
 }

shared_ptr<PhysicsController> PhysicsFactory::CreateKinematicBox(float width, float height, float depth, glm::vec3 pos, glm::quat quat)
{
	// Create the shape
	btCollisionShape * boxShape = new btBoxShape(btVector3(width, height, depth) * 0.50);
	btScalar mass = 0;
	btVector3 boxInertia(0,0,0);
	boxShape->calculateLocalInertia(mass,boxInertia);

	// This is a container for the box model
	shared_ptr<Box> box = make_shared<Box>(width, height, depth);
	box->worldMode = GameComponent::from_child;
	box->position = pos;
	Game::Instance()->Attach(box);

	// Create the rigid body
	btDefaultMotionState * boxMotionState = new btDefaultMotionState(btTransform(GLToBtQuat(quat)
		,GLToBtVector(pos)));			
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,  boxMotionState, boxShape, boxInertia);
	btRigidBody * body = new btRigidBody(fallRigidBodyCI);
	body->setFriction(567);
	dynamicsWorld->addRigidBody(body);

	// Create the physics component and add it to the box
	shared_ptr<PhysicsController> boxController = make_shared<PhysicsController>(PhysicsController(boxShape, body, boxMotionState));
	boxController->tag = "Box";
	body->setUserPointer(boxController.get());
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	box->Attach(boxController);

	return boxController;
}
shared_ptr<PhysicsController> PhysicsFactory::CreateCylinder(float radius, float height, glm::vec3 pos, glm::quat quat)
{
	// Create the shape
	btCollisionShape * shape = new btCylinderShape(btVector3(radius, height * 0.5f, radius));
	btScalar mass = 1;
	btVector3 inertia(0,0,0);
	shape->calculateLocalInertia(mass,inertia);

	// This is a container for the box model
	shared_ptr<GameComponent> cyl = make_shared<GameComponent>(Cylinder(radius, height));
	cyl->position = pos;
	Game::Instance()->Attach(cyl);

	// Create the rigid body
	btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(GLToBtQuat(quat)
		,GLToBtVector(pos)));			
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,  motionState, shape, inertia);
	btRigidBody * body = new btRigidBody(rigidBodyCI);
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	dynamicsWorld->addRigidBody(body);

	// Create the physics component and add it to the box
	shared_ptr<PhysicsController> component = make_shared<PhysicsController>(PhysicsController(shape, body, motionState));
	body->setUserPointer(component.get());
	component->tag = "Cylinder";
	cyl->Attach(component);

	return component;
}

shared_ptr<PhysicsController> PhysicsFactory::CreateRagDoll(glm::vec3 position)
{
        float depth = 0.3f;
        float torsoHeight = 1.5f;
        float torsoWidth = 1.0f;
        shared_ptr<PhysicsController> torso = CreateBox(torsoWidth, torsoHeight, depth, position, glm::quat());
        shared_ptr<PhysicsController> bodyPart;
        shared_ptr<PhysicsController> secondaryBodyPart;
        glm::quat q =  glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0));
        glm::vec3 offset;
        btHingeConstraint * hinge;
        btPoint2PointConstraint * ballAndSocket;

        //give the ragDoll a head
        offset = glm::vec3((torsoWidth/2 - 0.2),torsoHeight,0);
        bodyPart = CreateCylinder(0.2f,depth,position + offset,q);
        ballAndSocket = new btPoint2PointConstraint(*torso->rigidBody,*bodyPart->rigidBody, btVector3(0,0.75f,0),btVector3(-0.2,0,0));
        dynamicsWorld->addConstraint(ballAndSocket);
        
        //add an arm
        offset = glm::vec3(torsoWidth-0.25f,0,0);
        bodyPart = CreateCapsule(depth/2,(torsoHeight/2),position+offset,q);
        ballAndSocket = new btPoint2PointConstraint(*torso->rigidBody,*bodyPart->rigidBody, btVector3(-0.7,0.75,0),btVector3(0,0.3,0));
        dynamicsWorld->addConstraint(ballAndSocket);

        //add lower arm
        offset = glm::vec3(torsoWidth-0.25,-(torsoHeight/2),0);
        secondaryBodyPart = CreateCylinder(depth/2,(torsoHeight/3),position+offset,q);
        hinge = new btHingeConstraint(*bodyPart->rigidBody,*secondaryBodyPart->rigidBody, btVector3(0,-0.4,0), btVector3(0,0.4,0),btVector3(1,0,0), btVector3(1,0,0));
        dynamicsWorld->addConstraint(hinge);

        //add another arm
        offset = glm::vec3(-(torsoWidth-0.25f),0,0);
        bodyPart = CreateCapsule(depth/2,(torsoHeight/2),position+offset,q);
        ballAndSocket = new btPoint2PointConstraint(*torso->rigidBody,*bodyPart->rigidBody, btVector3(0.7,0.75,0),btVector3(0,0.3,0));
        dynamicsWorld->addConstraint(ballAndSocket);

        //add lower arm
        offset = glm::vec3(-(torsoWidth-0.25),-(torsoHeight/2),0);
        secondaryBodyPart = CreateCylinder(depth/2,(torsoHeight/3),position+offset,q);
        hinge = new btHingeConstraint(*bodyPart->rigidBody,*secondaryBodyPart->rigidBody, btVector3(0,-0.4,0), btVector3(0,0.4,0),btVector3(1,0,0), btVector3(1,0,0));
        dynamicsWorld->addConstraint(hinge);

        //add a leg
        offset = glm::vec3((torsoWidth/2 - 0.25f),torsoHeight,0);
        bodyPart = CreateCapsule(depth/2,(torsoHeight/2),position+offset,q);
        ballAndSocket = new btPoint2PointConstraint(*torso->rigidBody,*bodyPart->rigidBody, btVector3(0,-0.75,0),btVector3(0,0.75,0));
        dynamicsWorld->addConstraint(ballAndSocket);

        //add lower leg
        offset = glm::vec3(-(torsoWidth-0.25),-(torsoHeight/2),0);
        secondaryBodyPart = CreateCylinder(depth/2,(torsoHeight/3),position+offset,q);
        hinge = new btHingeConstraint(*bodyPart->rigidBody,*secondaryBodyPart->rigidBody, btVector3(0,-0.4,0), btVector3(0,0.4,0),btVector3(1,0,0), btVector3(1,0,0));
        dynamicsWorld->addConstraint(hinge);


        //add another leg
        offset = glm::vec3((torsoWidth/2 + 0.25f),torsoHeight,0);
        bodyPart = CreateCapsule(depth/2,(torsoHeight/2),position+offset,q);
        ballAndSocket = new btPoint2PointConstraint(*torso->rigidBody,*bodyPart->rigidBody, btVector3(0,-0.75,0),btVector3(0,0.75,0));
        dynamicsWorld->addConstraint(ballAndSocket);

        //add lower leg
        offset = glm::vec3((torsoWidth + 0.25),-(torsoHeight/2),0);
        secondaryBodyPart = CreateCylinder(depth/2,(torsoHeight/3),position+offset,q);
        hinge = new btHingeConstraint(*bodyPart->rigidBody,*secondaryBodyPart->rigidBody, btVector3(0,-0.4,0), btVector3(0,0.4,0),btVector3(1,0,0), btVector3(1,0,0));
        dynamicsWorld->addConstraint(hinge);

        torso->tag = "Ragdoll";
        return torso;
} 

shared_ptr<PhysicsController> PhysicsFactory::CreatePegBoard(glm::vec3 pos)
{
	glm::vec3 offset = glm::vec3(0,10,0);
	shared_ptr<PhysicsController> pegBoard = CreateKinematicBox(5,10,0.5,pos+offset,glm::quat());

	offset = glm::vec3(0,0,0.25f);
	shared_ptr<PhysicsController> peg  = CreateKinematicCylinder(1,2,glm::vec3(0),glm::quat());
	peg->worldMode = GameComponent::from_self_with_parent;
	peg->position +=  offset;
	pegBoard->Attach(peg);

	return pegBoard;
}
shared_ptr<PhysicsController> PhysicsFactory::CreateCameraPhysics()
{
	btVector3 inertia;
	// Now add physics to the camera
	btCollisionShape * cameraCyl = new btCylinderShape(btVector3(0.5f, 5.0f, 2.5f));
	cameraCyl->calculateLocalInertia(1, inertia);
	shared_ptr<PhysicsCamera> physicsCamera = make_shared<PhysicsCamera>(this);

	shared_ptr<Camera> camera = Game::Instance()->camera;
	camera->Attach(physicsCamera);

	btRigidBody::btRigidBodyConstructionInfo cameraCI(10,physicsCamera.get(), cameraCyl, inertia);  
	btRigidBody * body = new btRigidBody(cameraCI);
	physicsCamera->SetPhysicsStuff(cameraCyl, body, physicsCamera.get());
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	body->setActivationState(DISABLE_DEACTIVATION);

	dynamicsWorld->addRigidBody(body);
	return physicsCamera;
}

shared_ptr<PhysicsController> PhysicsFactory::CreateVehicle(glm::vec3 position)
{
	float width = 15;
	float height = 2;
	float length = 5;
	float wheelWidth = 1;
	float wheelRadius = 2;
	float wheelOffset = 2.0f;

	shared_ptr<PhysicsController> chassis = CreateBox(width, height, length, position, glm::quat());

	shared_ptr<PhysicsController> wheel;
	glm::quat q =  glm::angleAxis(glm::half_pi<float>(), glm::vec3(1, 0, 0));

	glm::vec3 offset;
	btHingeConstraint * hinge;

	offset = glm::vec3(- (width / 2 - wheelRadius), 0, - (length / 2 + wheelOffset));
	wheel = CreateCylinder(wheelRadius, wheelWidth, position + offset, q);	 
	hinge = new btHingeConstraint(* chassis->rigidBody, * wheel->rigidBody, GLToBtVector(offset),btVector3(0,0, 0), btVector3(0,0,1), btVector3(0,1,0), true);
	dynamicsWorld->addConstraint(hinge);

	offset = glm::vec3(+ (width / 2 - wheelRadius), 0, - (length / 2 + wheelOffset));
	wheel = CreateCylinder(wheelRadius, wheelWidth, glm::vec3(position.x + (width / 2) - wheelRadius, position.y, position.z - (length / 2) - wheelWidth), q);
	hinge = new btHingeConstraint(* chassis->rigidBody, * wheel->rigidBody, GLToBtVector(offset),btVector3(0,0, 0), btVector3(0,0,1), btVector3(0,1,0), true);
	dynamicsWorld->addConstraint(hinge);

	offset = glm::vec3(- (width / 2 - wheelRadius), 0, + (length / 2 + wheelOffset));
	wheel = CreateCylinder(wheelRadius, wheelWidth, position + offset, q);	 
	hinge = new btHingeConstraint(* chassis->rigidBody, * wheel->rigidBody, GLToBtVector(offset),btVector3(0,0, 0), btVector3(0,0,1), btVector3(0,1,0), true);
	dynamicsWorld->addConstraint(hinge);

	offset = glm::vec3(+ (width / 2 - wheelRadius), 0, + (length / 2 + wheelOffset));
	wheel = CreateCylinder(wheelRadius, wheelWidth, position + offset, q);	 
	hinge = new btHingeConstraint(* chassis->rigidBody, * wheel->rigidBody, GLToBtVector(offset),btVector3(0,0, 0), btVector3(0,0,1), btVector3(0,1,0), true);
	dynamicsWorld->addConstraint(hinge);

	return chassis;
}

shared_ptr<PhysicsController> PhysicsFactory::CreateGroundPhysics()
{
	shared_ptr<Ground> ground = make_shared<Ground>();

	btCollisionShape * groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
	btDefaultMotionState * groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));

	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
	btRigidBody* body = new btRigidBody(groundRigidBodyCI);
	body->setFriction(100);
	dynamicsWorld->addRigidBody(body);
	body->setUserPointer(ground.get());
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	shared_ptr<PhysicsController> groundComponent (new PhysicsController(groundShape, body, groundMotionState));
	groundComponent->tag = "Ground";
	ground->Attach(groundComponent);	
	Game::Instance()->SetGround(ground);
	return groundComponent;
}

shared_ptr<PhysicsController> PhysicsFactory::CreateRandomObject(glm::vec3 point, glm::quat q)
{
	vector<string> names;
	DIR * dir;
	struct dirent * ent;
	dir = opendir (Content::prefix.c_str());
	if (dir != NULL) 
	{
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) 
		{
			string fname = string(ent->d_name);
			int fpos = fname.find("objm");
			if (fpos != string::npos)
			{
				if ((fname.find("cube") == string::npos) && (fname.find("cyl") == string::npos) && (fname.find("sphere") == string::npos))
				{
					names.push_back(fname.substr(0, fpos - 1));
				}
			}
		}
		closedir (dir);
	} 
	else 
	{
		throw BGE::Exception("Could not list obj files in content folder");
	}

	int which = rand() % names.size();
	string name = names[which];
	return CreateFromModel(name, point, q, glm::vec3(3,3,3));
}


