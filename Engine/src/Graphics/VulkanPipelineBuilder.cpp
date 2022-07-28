#include "Graphics/VulkanPipelineBuilder.h"
#include "Core/Base.h"

namespace Engine {
	VkPipeline VulkanPipelineBuilder::BuildPipeline(VkDevice device, VkRenderPass renderPass) {
		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.pNext = nullptr;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &Viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &Scissor;
		
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.pNext = nullptr;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &ColorBlendAttachment;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.pNext = nullptr;
		pipelineInfo.stageCount = ShaderStages.size();
		pipelineInfo.pStages = ShaderStages.data();
		pipelineInfo.pVertexInputState = &VertexInputState;
		pipelineInfo.pInputAssemblyState = &InputAssemblyState;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &RasterizarionState;
		pipelineInfo.pMultisampleState = &MultisamnpleSate;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.layout = PipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		VkPipeline pipeline;
		VkResult result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);
		if (result != VK_SUCCESS) {
			ENGINE_ERROR("Failed to create Pipeline!");
			return VK_NULL_HANDLE;
		} 

		return pipeline;
	}
}