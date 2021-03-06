#pragma once
#include "GameComponent.h"
#include <vector>

namespace BGE
{
	class VectorDrawer :
		public GameComponent
	{
	public:
		VectorDrawer(glm::vec3 scale = glm::vec3(5,5,5));
		~VectorDrawer(void);

		std::vector<glm::vec3> lines;
		std::vector<glm::vec3> colours;

		void Draw();
	};
}
