#include "Capsule.h"
#include "model.h"
#include "Content.h"

using namespace BGE;

Capsule::Capsule(float height, float radius)
{
	shared_ptr<Model> model = Content::LoadModel("capsule");
	model->drawMode = Model::draw_modes::single_material;
	model->worldMode = GameComponent::from_parent;
	model->Initialise();
	diffuse = glm::vec3(RandomFloat(),RandomFloat(),RandomFloat());
	specular = glm::vec3(0,0,0);
	tag = "Capsule";
	Attach(model);
	scale = glm::vec3(radius, height+radius, radius);
}


Capsule::~Capsule(void)
{
}
