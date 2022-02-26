#include "Graphics/VulkanContext.h"
#include "Graphics/VulkanHelper.h"
#include "Graphics/VulkanFactory.h"
#include "Core/Core.h"

#include <VkBootstrap.h>

namespace Engine {
	void VulkanContext::Initialize() {
		InitVulkan();
		InitSwapchain();
		InitCommands();
		InitDefaultRenderPass();
		InitFrameBuffers();
		InitSyncObjects();
		InitPipeline();
	}

	VulkanContext::~VulkanContext() {
		vkDestroyFence(Device, RenderFence, nullptr);
		vkDestroySemaphore(Device, RenderSemaphore, nullptr);
		vkDestroySemaphore(Device, PresentSemaphore, nullptr);

		vkDestroyCommandPool(Device, CommandPool, nullptr);

		vkDestroySwapchainKHR(Device, Swapchain, nullptr);

		vkDestroyRenderPass(Device, RenderPass, nullptr);

		for (int i = 0; i < SwapchainImageViews.size(); i++) {
			vkDestroyFramebuffer(Device, FrameBuffers[i], nullptr);
			vkDestroyImageView(Device, SwapchainImageViews[i], nullptr);
		}

		vkDestroyDevice(Device, nullptr);
		vkDestroySurfaceKHR(Instance, Surface, nullptr);
		vkb::destroy_debug_utils_messenger(Instance, DebugMessenger, nullptr);
		vkDestroyInstance(Instance, nullptr);
	}

	void VulkanContext::InitVulkan() {
		vkb::InstanceBuilder builder;

		auto inst = builder
			.set_app_name(Core::Get().Settings.App.Name.c_str())
			.set_engine_version(ENGINE_VERSION_MAJOR, ENGINE_VERSION_MINOR, ENGINE_VERSION_PATCH)
			.request_validation_layers(true)
			.require_api_version(1, 1, 0)
			.use_default_debug_messenger()
			.build();

		Instance = inst.value().instance;
		DebugMessenger = inst.value().debug_messenger;

		VkResult result = glfwCreateWindowSurface(Instance, Core::Get().GetWindow(), nullptr, &Surface);
		VK_ABORT_ON_FAIL(result, "Could not create surface");

		vkb::PhysicalDeviceSelector selector(inst.value());
		vkb::PhysicalDevice vkbPhysicalDevice = selector
			.set_minimum_version(1, 1)
			.set_surface(Surface)
			.select()
			.value();
		PhysicalDevice = vkbPhysicalDevice.physical_device;

		vkb::DeviceBuilder deviceBuilder(vkbPhysicalDevice);
		vkb::Device vkbDevice = deviceBuilder.build().value();
		Device = vkbDevice.device;

		GrphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
		GrphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
	}

	void VulkanContext::InitSwapchain() {
		vkb::SwapchainBuilder swapchainBuilder(PhysicalDevice, Device, Surface);
		vkb::Swapchain vkbSwapchain = swapchainBuilder
			.use_default_format_selection()
			.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
			.set_desired_extent(Core::Get().GetWidth(), Core::Get().GetHeight())
			.build()
			.value();

		Swapchain = vkbSwapchain.swapchain;
		SwapchainImages = vkbSwapchain.get_images().value();
		SwapchainImageViews = vkbSwapchain.get_image_views().value();
		SwapchainExtend = vkbSwapchain.extent;
		SwapchainImageFormat = vkbSwapchain.image_format;
	}

	void VulkanContext::InitCommands() {
		VkResult result = VulkanFactory::CreateCommandPool(Device, &CommandPool, GrphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		VK_ABORT_ON_FAIL(result, "Could not create Command Pool");

		result = VulkanFactory::CreateCommandBuffer(Device, &CommandBuffer, CommandPool);
		VK_ABORT_ON_FAIL(result, "Could not allocate main command buffer");
	}
	
	void VulkanContext::InitDefaultRenderPass() {
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = SwapchainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.pNext = nullptr;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		VkResult result = vkCreateRenderPass(Device, &renderPassInfo, nullptr, &RenderPass);
		VK_ABORT_ON_FAIL(result, "Could not create default render pass");
	}

	void VulkanContext::InitFrameBuffers() {
		VkFramebufferCreateInfo fbInfo{};
		fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbInfo.pNext = nullptr;
		fbInfo.renderPass = RenderPass;
		fbInfo.attachmentCount = 1;
		fbInfo.width = Core::Get().GetWidth();
		fbInfo.height = Core::Get().GetHeight();
		fbInfo.layers = 1;

		const uint32_t nSwapchainImages = SwapchainImages.size();
		FrameBuffers.resize(nSwapchainImages);

		for (int i = 0; i < nSwapchainImages; i++) {
			fbInfo.pAttachments = &SwapchainImageViews[i];
			VkResult result = vkCreateFramebuffer(Device, &fbInfo, nullptr, &FrameBuffers[i]);
			VK_ABORT_ON_FAIL(result, "Could not create framebuffer");
		}
	}

	void VulkanContext::InitSyncObjects() {
		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.pNext = nullptr;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VkResult result = vkCreateFence(Device, &fenceCreateInfo, nullptr, &RenderFence);
		VK_ABORT_ON_FAIL(result, "Could not create Render Fence");

		VkSemaphoreCreateInfo semaphoreCreateInfo{};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreCreateInfo.pNext = nullptr;
		semaphoreCreateInfo.flags = 0;

		result = vkCreateSemaphore(Device, &semaphoreCreateInfo, nullptr, &PresentSemaphore);
		VK_ABORT_ON_FAIL(result, "Could not create Present Semaphore");
		result = vkCreateSemaphore(Device, &semaphoreCreateInfo, nullptr, &RenderSemaphore);
		VK_ABORT_ON_FAIL(result, "Could not create Render Semaphore");
	}

	void VulkanContext::InitPipeline() {
		VkShaderModule triangleFragShader;
		VkShaderModule triangleVertShader;
		bool result = VulkanFactory::CreateShaderFromFile(Device, "triangle.frag.spv", &triangleFragShader);
		ENGINE_ASSERT(result, "Could not load Traingle Fragment Shader");

		result = VulkanFactory::CreateShaderFromFile(Device, "triangle.vert.spv", &triangleVertShader);
		ENGINE_ASSERT(result, "Could not load Traingle Vertex Shader");

	}
}