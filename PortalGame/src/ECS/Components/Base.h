#ifndef PGAME_ECS_COMPONENTS_BASE_H
#define PGAME_ECS_COMPONENTS_BASE_H

#include <glm/glm.hpp>

namespace PGame {
	namespace ECS {
		struct Transform {
			glm::vec3 Translation;
			glm::vec3 Rotation;
			glm::vec3 Scale;
		};
	}
}


#endif