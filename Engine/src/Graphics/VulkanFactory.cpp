#include "Core/Base.h"
#include "Graphics/VulkanFactory.h"
#include "Graphics/VulkanHelper.h"

#include <fstream>

namespace Engine {
	VkResult VulkanFactory::CreateCommandPool(VkDevice device, VkCommandPool* out_CommandPool, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags /* = 0 */) {
		ENGINE_DEBUG_ALLOC("Creating Command Pool");
		VkCommandPoolCreateInfo commandPoolInfo = {};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.pNext = nullptr;

		commandPoolInfo.queueFamilyIndex = queueFamilyIndex;
		commandPoolInfo.flags = flags;

		return vkCreateCommandPool(device, &commandPoolInfo, nullptr, out_CommandPool);
	}

	VkResult VulkanFactory::CreateCommandBuffer(VkDevice device, VkCommandBuffer* out_CommandBuffer, VkCommandPool commandPool, uint32_t count /* = 1 */, VkCommandBufferLevel level /* = VK_COMMAND_BUFFER_LEVEL_PRIMARY */) {
		ENGINE_DEBUG_ALLOC("Creating " << count << " Command Buffer(s)");
		VkCommandBufferAllocateInfo commandBufferInfo = {};
		commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferInfo.pNext = nullptr;
		commandBufferInfo.commandPool = commandPool;
		commandBufferInfo.commandBufferCount = count;
		commandBufferInfo.level = level;

		return vkAllocateCommandBuffers(device, &commandBufferInfo, out_CommandBuffer);
	}

	bool VulkanFactory::CreateShaderFromFile(VkDevice device, const std::string& filePath, VkShaderModule* out_ShaderModule) {
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			ENGINE_ERROR("Could not open Shader File: \"" << filePath << "\"");
			return false;
		}

		size_t fileSize = file.tellg();
		std::vector<char> buffer(fileSize);
		
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.codeSize = buffer.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

		return VK_CHECK_RESULT(vkCreateShaderModule(device, &createInfo, nullptr, out_ShaderModule));
	}
}