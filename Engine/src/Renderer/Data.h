#pragma once

#include <glm/glm.hpp>
#include "OpenGL/Types.h"

namespace Engine {
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

		struct MeshInfo {
			GLuint InstanceBufferIndex;
			GLuint VertexBufferIndex;
			GLuint CommandBufferOffset;
		};

		struct DrawArraysIndirectCommand {
			GLuint VertexCount;
			GLuint InstanceCount;
			GLuint FirstIndex;
			GLuint BaseInstance;
		};
	}
}