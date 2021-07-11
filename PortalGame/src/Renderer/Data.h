#pragma once

#include <glm/glm.hpp>

namespace PGame {
	namespace Renderer {
		struct Vertex {
			glm::vec3 pos;
			glm::vec2 uv;
			glm::vec3 normal;
		};

		typedef  struct {
			unsigned int  count;
			unsigned int  instanceCount;
			unsigned int  firstIndex;
			unsigned int  baseInstance;
		} DrawArraysIndirectCommand;
	}
}