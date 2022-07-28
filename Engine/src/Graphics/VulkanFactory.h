#pragma once

#include <vulkan/vulkan.h>
#include <string>

namespace Engine {
	namespace VulkanFactory {
		VkResult CreateCommandPool(VkDevice device, VkCommandPool* out_CommandPool, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);
		VkResult CreateCommandBuffer(VkDevice device, VkCommandBuffer* out_CommandBuffer, VkCommandPool commandPool, uint32_t count = 1, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	
		bool CreateShaderFromFile(VkDevice device, const std::string& filePath, VkShaderModule* out_ShaderModule);

		VkPipelineShaderStageCreateInfo ShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
		VkPipelineVertexInputStateCreateInfo VertexInputStateCreateInfo();
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyCreateInfo(VkPrimitiveTopology topology);
		VkPipelineRasterizationStateCreateInfo RastzerizationStateCreateInfo(VkPolygonMode polygonMode);
		VkPipelineMultisampleStateCreateInfo MultisampleSateCreateInfo();
		VkPipelineColorBlendAttachmentState ColorBlendAttachmentState();
		VkResult CreatePipelineLayout(VkDevice device, VkPipelineLayout* out_PipelineLayout);
	}
}