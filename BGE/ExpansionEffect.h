#pragma once
#include"ParticleEffect.h"

namespace BGE
{
	class ExpansionEffect : public ParticleEffect
	{
	public:
		ExpansionEffect(void);
		~ExpansionEffect(void);
		bool Initialise();
		void Update(float timeDelta);
		void InitParticle(Particle & particle);
		void UpdateParticle(float timeDelta, Particle & particle);
		int numParticals;
		float theta;
		float phi;
		float phiIncrement;
		float thetaIncrement;
		
	};
}
