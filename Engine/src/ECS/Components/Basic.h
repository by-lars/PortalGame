#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Engine {
	namespace ECS {
		struct Transform {
			Transform() : Translation(0,0,0), Rotation(0,0,0), Scale(1,1,1) {}

			glm::vec3 Translation;
			glm::vec3 Rotation;
			glm::vec3 Scale;
		};

		struct Tag {
			std::string Tag;
		};
	}
}

