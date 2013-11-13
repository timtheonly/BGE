#include "ExpansionEffect.h"
#include "utils.h"
using namespace BGE;

bool ExpansionEffect::Initialise()
{
	for(int i=0;i<numParticals;i++)
	{
		Particle p;
		InitParticle(p);
		if(i %10  == 0)
		{
			row++;	
			angle=0.0f;
		}
		particles.push_back(p);
	}
	return ParticleEffect::Initialise();
}

ExpansionEffect::ExpansionEffect(void)
{
	numParticals = 20;
	angle =0.0f;
	row = 1;
	angleIncrement = (glm::pi<float>()*2)/numParticals;
	diffuse = glm::vec3(0,0,1);
}

void ExpansionEffect::InitParticle(Particle & p)
{
	float radius = 0.75f;
	angle += angleIncrement;

	p.position.x =  (row*0.25f) * glm::cos(angle);
	p.position.z =  (row*0.25f) * glm::sin(angle);
	p.position.y = row * 0.5;

	p.diffuse.a = 1.0f;
	p.size = 10;

}

void ExpansionEffect::UpdateParticle(float timeDelta, Particle & p)
{
	glm::vec3 distance = p.position - position;
	glm::vec3 direction = distance;
	direction.y=0;
	direction = glm::normalize(direction);

	p.velocity = glm::vec3(10);
	p.velocity *= direction;
	
	p.position += p.velocity * timeDelta;

	
	if(glm::length(distance) > 100)
	{
		InitParticle(p);
	}
}

void ExpansionEffect::Update(float timeDelta)
{
	ParticleEffect::Update(timeDelta);
}

ExpansionEffect::~ExpansionEffect(void)
{
}
