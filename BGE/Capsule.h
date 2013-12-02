#pragma once
#include "gamecomponent.h"

namespace BGE
{
	class Capsule :
		public GameComponent
	{
	public:
		Capsule(float height, float radius);
		~Capsule(void);
	};

}