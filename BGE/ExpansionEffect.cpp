#include "ExpansionEffect.h"
#include "utils.h"
using namespace BGE;

bool ExpansionEffect::Initialise()
{
	for(int i=0;i<numParticals;i++)
	{
		Particle p;
		InitParticle(p);
		particles.push_back(p);
	}
	return ParticleEffect::Initialise();
}


void ExpansionEffect::InitParticle(Particle & p)
{
	float radius = 0.75f;
	if(angle > (glm::pi<float>()*2))
	{
		angle = 0.0f;
	}
	angle += angleIncrement;

	p.position.x =  1.5 * glm::cos(angle);
	p.position.z =  1.5 * glm::sin(angle);

	p.diffuse.a = 1.0f;
	p.age = 0;
	p.alive = true;
	p.size = 50;
	p.lifetime = 10.0f;
}

void ExpansionEffect::UpdateParticle(float timeDelta, Particle & p)
{
	glm::vec3 direction = p.position  - position;
	direction.y=0;
	direction = glm::normalize(direction);

	p.velocity = glm::vec3(10);
	p.velocity *= direction;
	
	p.position += p.velocity * timeDelta;

	glm::vec3 distance = p.position - position;
	if(glm::length(distance) > 10)
	{
		InitParticle(p);
	}
}

void ExpansionEffect::Update(float timeDelta)
{
	ParticleEffect::Update(timeDelta);
}

ExpansionEffect::ExpansionEffect(void)
{
	numParticals = 10;
	angle =0.0f;
	angleIncrement = (glm::pi<float>()*2)/numParticals;
	diffuse = glm::vec3(0,0,1);
}


ExpansionEffect::~ExpansionEffect(void)
{
}
