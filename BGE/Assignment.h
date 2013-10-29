#pragma once
#include "Game.h"
#include "GameComponent.h"
#include "FountainEffect.h"

using namespace std;

namespace BGE
{
	class Assignment : public  Game
	{
	public:
		float effectTheta;
		float effectYSpeed;
		shared_ptr<FountainEffect> hatFountain;
		shared_ptr<GameComponent> hat;
		Assignment(void);
		~Assignment(void);
		bool Initialise();
		void Update(float timeDelta);
	};

}