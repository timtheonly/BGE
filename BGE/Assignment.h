#pragma once
#include "Game.h"
#include "GameComponent.h"

using namespace std;

namespace BGE
{
	class Assignment : public  Game
	{
	public:
		shared_ptr<GameComponent> hat;
		Assignment(void);
		~Assignment(void);
		bool Initialise();
		void Update(float timeDelta);
	};

}