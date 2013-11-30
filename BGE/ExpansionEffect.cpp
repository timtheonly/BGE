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

ExpansionEffect::ExpansionEffect(void)
{
	numParticals = 1000;
	theta  = glm::pi<float>();
	phi = 0;
	thetaIncrement = (glm::pi<float>()*2)/numParticals;
	phiIncrement = 0.1f;
	diffuse = glm::vec3(0,0,1);
}

void ExpansionEffect::InitParticle(Particle & p)
{
	float radius = 3.5f;
	theta += thetaIncrement;
	phi += phiIncrement;

	p.diffuse = glm::vec4(RandomClamped(0,1),RandomClamped(0,0.5f),1,1);
	p.position.x =   glm::cos(theta) * glm::cos(phi) *radius;
	p.position.y =  glm::sin(phi) *radius;
	p.position.z =  glm::cos(theta)* glm::sin(phi) * radius;

	p.size = RandomClamped(10,15);

}

void ExpansionEffect::UpdateParticle(float timeDelta, Particle & p)
{
	glm::vec3 distance = p.position - position;
	glm::vec3 direction = distance;
	direction = glm::normalize(direction);

	p.velocity = glm::vec3(10);
	p.velocity *= direction;
	p.position += p.velocity * timeDelta;

	if(glm::length(distance) > 5)
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
