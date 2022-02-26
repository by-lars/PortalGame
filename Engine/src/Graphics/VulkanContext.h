#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace Engine {
	class VulkanContext {
	public:
		VulkanContext() = default;
		~VulkanContext();

		void Initialize();

		VkInstance Instance;
		VkDebugUtilsMessengerEXT DebugMessenger;
		VkPhysicalDevice PhysicalDevice;
		VkDevice Device;
		VkSurfaceKHR Surface;
		VkExtent2D SwapchainExtend;
		VkSwapchainKHR Swapchain;
		VkFormat SwapchainImageFormat;
		std::vector<VkImage> SwapchainImages;
		std::vector<VkImageView> SwapchainImageViews;

		VkQueue GrphicsQueue;
		uint32_t GrphicsQueueFamily;
		VkCommandPool CommandPool;
		VkCommandBuffer CommandBuffer;

		std::vector<VkFramebuffer> FrameBuffers;
		VkRenderPass RenderPass;

		VkSemaphore PresentSemaphore, RenderSemaphore;
		VkFence RenderFence;

	private:
		void InitVulkan();
		void InitSwapchain();
		void InitCommands();
		void InitDefaultRenderPass();
		void InitFrameBuffers();
		void InitSyncObjects();
		void InitPipeline();
	};
}