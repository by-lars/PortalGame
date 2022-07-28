#include "Graphics/VkMesh.h"

namespace Engine {
	VertexInputDescription Vertex::GetVertexDescription() {
		VertexInputDescription description;
		
		VkVertexInputBindingDescription mainBinding{};
		mainBinding.binding = 0;
		mainBinding.stride = sizeof(Vertex);
		mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		description.Bidnings.push_back(mainBinding);

		VkVertexInputAttributeDescription positionAttribute{};
		positionAttribute.binding = 0;
		positionAttribute.location = 0;
		positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
		positionAttribute.offset = offsetof(Vertex, Position);

		VkVertexInputAttributeDescription normalAttribute{};
		normalAttribute.binding = 0;
		normalAttribute.location = 1;
		normalAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
		normalAttribute.offset = offsetof(Vertex, Normal);

		VkVertexInputAttributeDescription colorAttribute{};
		colorAttribute.binding = 0;
		colorAttribute.location = 2;
		colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
		colorAttribute.offset = offsetof(Vertex, Color);

		description.Attributes.push_back(positionAttribute);
		description.Attributes.push_back(normalAttribute);
		description.Attributes.push_back(colorAttribute);

		return description;
	}
}