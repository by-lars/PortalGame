#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace Engine {
	class VulkanPipelineBuilder {
	public:
		VkPipeline BuildPipeline(VkDevice device, VkRenderPass pass);

		std::vector<VkPipelineShaderStageCreateInfo> ShaderStages;
		VkPipelineVertexInputStateCreateInfo VertexInputState;
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyState;
		VkPipelineRasterizationStateCreateInfo RasterizarionState;
		VkPipelineColorBlendAttachmentState ColorBlendAttachment;
		VkPipelineMultisampleStateCreateInfo MultisamnpleSate;
		VkPipelineLayout PipelineLayout;
		VkViewport Viewport;
		VkRect2D Scissor;
	};
}