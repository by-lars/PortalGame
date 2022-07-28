#pragma once

#include "Graphics/DeletionQueue.h"
#include "Graphics/VkMesh.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <vk_mem_alloc.h>

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

		VkPipelineLayout TrianglePipelineLayout;
		VkPipeline TrianglePipeline;
		VkPipeline RedTrianglePipeline;

		VkPipeline MeshPipeline;
		Mesh TriangleMesh;
	private:
		void LoadMeshes();
		void UploadMesh(Mesh& mesh);

		VmaAllocator m_Allocator;
		VkDeletionQueue m_DeletionQueue;
	};
}