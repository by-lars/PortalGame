#pragma once

#include "Core/Base.h"
#include <vulkan/vulkan.h>
#include <stdlib.h>
#include <vk_mem_alloc.h>

namespace Engine {
	struct VkAllocatedBuffer {
		VkBuffer Buffer;
		VmaAllocation Allocation;
	};

	inline void VK_ABORT_ON_FAIL(VkResult result, const std::string& msg) {
		if (result != VK_SUCCESS) {
			ENGINE_ERROR("Vulkan error: " << result << " - " << msg);
			abort();
		}
	}

	inline bool VK_CHECK_RESULT(VkResult result) {
		if (result == VK_SUCCESS) {
			return true;
		}
		else {
			ENGINE_ERROR("Vulkan error: " << result);
			return false;
		}
	}

	template<typename T> VkObjectType GetVulkanType();
	template<> inline VkObjectType GetVulkanType<VkInstance>() { return VK_OBJECT_TYPE_INSTANCE; }
	template<> inline VkObjectType GetVulkanType<VkDebugUtilsMessengerEXT>() { return VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT; }
	template<> inline VkObjectType GetVulkanType<VkSurfaceKHR>() { return VK_OBJECT_TYPE_SURFACE_KHR; }
	template<> inline VkObjectType GetVulkanType<VkDevice>() { return VK_OBJECT_TYPE_DEVICE; }
	template<> inline VkObjectType GetVulkanType<VkFramebuffer>() { return VK_OBJECT_TYPE_FRAMEBUFFER; }
	template<> inline VkObjectType GetVulkanType<VkImageView>() { return VK_OBJECT_TYPE_IMAGE_VIEW; }
	template<> inline VkObjectType GetVulkanType<VkImage>() { return VK_OBJECT_TYPE_IMAGE; }
	template<> inline VkObjectType GetVulkanType<VkSwapchainKHR>() { return VK_OBJECT_TYPE_SWAPCHAIN_KHR; }
	template<> inline VkObjectType GetVulkanType<VkRenderPass>() { return VK_OBJECT_TYPE_RENDER_PASS; }
	template<> inline VkObjectType GetVulkanType<VkShaderModule>() { return VK_OBJECT_TYPE_SHADER_MODULE; }
	template<> inline VkObjectType GetVulkanType<VkCommandPool>() { return VK_OBJECT_TYPE_COMMAND_POOL; }
	template<> inline VkObjectType GetVulkanType<VkFence>() { return VK_OBJECT_TYPE_FENCE; }
	template<> inline VkObjectType GetVulkanType<VkSemaphore>() { return VK_OBJECT_TYPE_SEMAPHORE; }
	template<> inline VkObjectType GetVulkanType<VkPipeline>() { return VK_OBJECT_TYPE_PIPELINE; }
	template<> inline VkObjectType GetVulkanType<VkPipelineLayout>() { return VK_OBJECT_TYPE_PIPELINE_LAYOUT; }
}
