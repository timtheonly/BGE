#include "Assignment.h"
#include "Content.h"
using namespace BGE;

Assignment::Assignment(void)
{
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
	Attach(hat);

	fullscreen = false;
	width = 800;
	height = 600;

	Game::Initialise();

	camera->GetController()->position = glm::vec3(0, 4, 20);
	return true;
}

void Assignment::Update(float gameTime)
{
	Game::Update(gameTime);
}