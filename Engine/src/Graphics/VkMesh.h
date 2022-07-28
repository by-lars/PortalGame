#pragma once

#include "Graphics/VulkanHelper.h"
#include <glm/glm.hpp>
#include <vector>


namespace Engine {
	struct VertexInputDescription {
		std::vector<VkVertexInputBindingDescription> Bidnings;
		std::vector<VkVertexInputAttributeDescription> Attributes;
		VkPipelineVertexInputStateCreateFlags Flags = 0;
	};

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Color;

		static VertexInputDescription GetVertexDescription();
	};

	struct Mesh {
		std::vector<Vertex> Vertecies;
		VkAllocatedBuffer VertexBuffer;
	};
}