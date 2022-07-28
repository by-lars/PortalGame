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

	VkPipelineShaderStageCreateInfo VulkanFactory::ShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule) {
		VkPipelineShaderStageCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		info.pNext = nullptr;
		info.stage = stage;
		info.module = shaderModule;
		info.pName = "main";
		return info;
	}

	VkPipelineVertexInputStateCreateInfo VulkanFactory::VertexInputStateCreateInfo() {
		VkPipelineVertexInputStateCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		info.pNext = nullptr;
		info.vertexBindingDescriptionCount = 0;
		info.vertexAttributeDescriptionCount = 0;
		return info;
	}

	VkPipelineInputAssemblyStateCreateInfo VulkanFactory::InputAssemblyCreateInfo(VkPrimitiveTopology topology) {
		VkPipelineInputAssemblyStateCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		info.pNext = nullptr;
		info.topology = topology;
		info.primitiveRestartEnable = VK_FALSE;
		return info;
	}

	VkPipelineRasterizationStateCreateInfo VulkanFactory::RastzerizationStateCreateInfo(VkPolygonMode polygonMode) {
		VkPipelineRasterizationStateCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		info.pNext = nullptr;
		info.depthClampEnable = VK_FALSE;
		info.rasterizerDiscardEnable = VK_FALSE;
		info.polygonMode = polygonMode;
		info.lineWidth = 1.0f;
		info.cullMode = VK_CULL_MODE_NONE;
		info.frontFace = VK_FRONT_FACE_CLOCKWISE;
		info.depthBiasEnable = VK_FALSE;
		info.depthBiasConstantFactor = 0.0f;
		info.depthBiasClamp = 0.0f;
		info.depthBiasSlopeFactor = 0.0f;
		return info;
	}

	VkPipelineMultisampleStateCreateInfo VulkanFactory::MultisampleSateCreateInfo() {
		VkPipelineMultisampleStateCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		info.pNext = nullptr;
		info.sampleShadingEnable = VK_FALSE;
		info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		info.minSampleShading = 1.0f;
		info.pSampleMask = nullptr;
		info.alphaToCoverageEnable = VK_FALSE;
		info.alphaToOneEnable = VK_FALSE;
		return info;
	}

	VkPipelineColorBlendAttachmentState VulkanFactory::ColorBlendAttachmentState() {
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		return colorBlendAttachment;
	}

	VkResult VulkanFactory::CreatePipelineLayout(VkDevice device, VkPipelineLayout* out_PipelineLayout) {
		VkPipelineLayoutCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.setLayoutCount = 0;
		info.pSetLayouts = nullptr;
		info.pushConstantRangeCount = 0;
		info.pPushConstantRanges = nullptr;

		return vkCreatePipelineLayout(device, &info, nullptr, out_PipelineLayout);
	}
}