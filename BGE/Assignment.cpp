#include "Assignment.h"
#include "Content.h"
using namespace BGE;

Assignment::Assignment(void)
{
	effectYSpeed = 0.5f;
	effectTheta = 0;
}


Assignment::~Assignment(void)
{
}

bool Assignment::Initialise()
{
	std::shared_ptr<GameComponent> ground = make_shared<Ground>();
	Attach(ground);

	hat = make_shared<GameComponent>();
	hat->Attach(Content::LoadModel("hat"));
	hat->position = glm::vec3(0,1,-1);
	hat->diffuse= glm::vec3(0.0f,0.0f,1.0f);
	Attach(hat);

	fullscreen = false;
	width = 800;
	height = 600;

	hatFountain = make_shared<ExpansionEffect>();
	hatFountain->position = hat->position;
	hatFountain->diffuse = glm::vec3(1,0,1);
	Attach(hatFountain);
	Game::Initialise();

	camera->GetController()->position = glm::vec3(0, 4, 20);
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
	
	effectTheta += gameTime;
	if(effectTheta >= glm::pi<float>() *2)
	{
		effectTheta = 0;
	}
	/*
	hatFountain->position.x = glm::cos(effectTheta);
	hatFountain->position.z = glm::sin(effectTheta);
	hatFountain->position.y += effectYSpeed * gameTime;*/
	Game::Update(gameTime);
}