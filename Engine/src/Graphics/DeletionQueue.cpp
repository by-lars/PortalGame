#include "Graphics/DeletionQueue.h"
#include <VkBootstrap.h>

namespace Engine {
	void VkDeletionQueue::Initialize(VkInstance instance, VkDevice device) {
		m_Instance = instance;
		m_Device = device;
	}

	void VkDeletionQueue::DeleteAll() {
		for (auto it = m_Objects.rbegin(); it != m_Objects.rend(); it++) {
			switch (it->Type) {
				case VK_OBJECT_TYPE_INSTANCE:					vkDestroyInstance((VkInstance)it->Handle, nullptr); break;
				case VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT:  vkb::destroy_debug_utils_messenger(m_Instance, (VkDebugUtilsMessengerEXT)it->Handle, nullptr); break;
				case VK_OBJECT_TYPE_SURFACE_KHR:				vkDestroySurfaceKHR(m_Instance, (VkSurfaceKHR)it->Handle, nullptr); break;
				case VK_OBJECT_TYPE_DEVICE:						vkDestroyDevice((VkDevice)it->Handle, nullptr); break;
				case VK_OBJECT_TYPE_FRAMEBUFFER:				vkDestroyFramebuffer(m_Device, (VkFramebuffer)it->Handle, nullptr); break;
				case VK_OBJECT_TYPE_IMAGE_VIEW:					vkDestroyImageView(m_Device, (VkImageView)it->Handle, nullptr); break;
				case VK_OBJECT_TYPE_IMAGE:						vkDestroyImage(m_Device, (VkImage)it->Handle, nullptr); break;
				case VK_OBJECT_TYPE_SWAPCHAIN_KHR:				vkDestroySwapchainKHR(m_Device, (VkSwapchainKHR)it->Handle, nullptr); break;
				case VK_OBJECT_TYPE_RENDER_PASS:				vkDestroyRenderPass(m_Device, (VkRenderPass)it->Handle, nullptr); break;
				case VK_OBJECT_TYPE_SHADER_MODULE:				vkDestroyShaderModule(m_Device, (VkShaderModule)it->Handle, nullptr); break;
				case VK_OBJECT_TYPE_COMMAND_POOL:				vkDestroyCommandPool(m_Device, (VkCommandPool)it->Handle, nullptr); break;
				case VK_OBJECT_TYPE_FENCE:						vkDestroyFence(m_Device, (VkFence)it->Handle, nullptr); break;
				case VK_OBJECT_TYPE_SEMAPHORE:					vkDestroySemaphore(m_Device, (VkSemaphore)it->Handle, nullptr); break;
				case VK_OBJECT_TYPE_PIPELINE:					vkDestroyPipeline(m_Device, (VkPipeline)it->Handle, nullptr); break;
				case VK_OBJECT_TYPE_PIPELINE_LAYOUT:			vkDestroyPipelineLayout(m_Device, (VkPipelineLayout)it->Handle, nullptr); break;

				default: ENGINE_ASSERT(false, "Unknown Object Type! " << it->Type); break;
			}
		}
	}
}