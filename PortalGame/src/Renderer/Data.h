#pragma once

#include <glm/glm.hpp>

namespace PGame {
	namespace Renderer {
		struct Vertex {
			glm::vec3 Pos;
			glm::vec2 Uv;
			glm::vec3 Normal;
		};
		
		struct Mesh {
			std::vector<Vertex> Vertecies;
			inline uint32_t GetSizeInBytes() const {
				return sizeof(Vertecies) * Vertecies.size();
			}
		};

		struct DrawArraysIndirectCommand {
			unsigned int VertexCount;
			unsigned int InstanceCount;
			unsigned int FirstIndex;
			unsigned int BaseInstance;
		};
	}
}