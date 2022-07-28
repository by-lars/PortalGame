#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include "Graphics/VulkanContext.h"
#include "Graphics/VulkanHelper.h"
#include "Graphics/VulkanFactory.h"
#include "Graphics/VulkanPipelineBuilder.h"
#include "Core/Core.h"

#include <VkBootstrap.h>

namespace Engine {
	void VulkanContext::Initialize() {
		VkResult result;

		//
		// Initialize Vulkan Instance
		//
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

		result = glfwCreateWindowSurface(Instance, Core::Get().GetWindow(), nullptr, &Surface);
		VK_ABORT_ON_FAIL(result, "Could not create surface");
		m_DeletionQueue.PushBack(Surface);

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
		m_DeletionQueue.Initialize(Instance, Device);

		GrphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
		GrphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
	
		//
		// Init Swap Chain
		//
		vkb::SwapchainBuilder swapchainBuilder(PhysicalDevice, Device, Surface);
		vkb::Swapchain vkbSwapchain = swapchainBuilder
			.use_default_format_selection()
			.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
			.set_desired_extent(Core::Get().GetWidth(), Core::Get().GetHeight())
			.build()
			.value();

		Swapchain = vkbSwapchain.swapchain;
		m_DeletionQueue.PushBack(Swapchain);

		SwapchainImages = vkbSwapchain.get_images().value();
		SwapchainImageViews = vkbSwapchain.get_image_views().value();
		SwapchainExtend = vkbSwapchain.extent;
		SwapchainImageFormat = vkbSwapchain.image_format;

		//
		// Init Command Buffers
		//
		result = VulkanFactory::CreateCommandPool(Device, &CommandPool, GrphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		VK_ABORT_ON_FAIL(result, "Could not create Command Pool");
		m_DeletionQueue.PushBack(CommandPool);

		result = VulkanFactory::CreateCommandBuffer(Device, &CommandBuffer, CommandPool);
		VK_ABORT_ON_FAIL(result, "Could not allocate main command buffer");

		//
		// Init Default Render Pass
		//
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

		result = vkCreateRenderPass(Device, &renderPassInfo, nullptr, &RenderPass);
		VK_ABORT_ON_FAIL(result, "Could not create default render pass");
		m_DeletionQueue.PushBack(RenderPass);

		//
		// Init Frame Buffers
		//
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
			result = vkCreateFramebuffer(Device, &fbInfo, nullptr, &FrameBuffers[i]);
			VK_ABORT_ON_FAIL(result, "Could not create framebuffer");
			m_DeletionQueue.PushBack(FrameBuffers[i]);
			m_DeletionQueue.PushBack(SwapchainImageViews[i]);
		}

		//
		// Init Sync Objects
		//
		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.pNext = nullptr;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		result = vkCreateFence(Device, &fenceCreateInfo, nullptr, &RenderFence);
		VK_ABORT_ON_FAIL(result, "Could not create Render Fence");
		m_DeletionQueue.PushBack(RenderFence);

		VkSemaphoreCreateInfo semaphoreCreateInfo{};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreCreateInfo.pNext = nullptr;
		semaphoreCreateInfo.flags = 0;

		result = vkCreateSemaphore(Device, &semaphoreCreateInfo, nullptr, &PresentSemaphore);
		VK_ABORT_ON_FAIL(result, "Could not create Present Semaphore");
		m_DeletionQueue.PushBack(PresentSemaphore);

		result = vkCreateSemaphore(Device, &semaphoreCreateInfo, nullptr, &RenderSemaphore);
		VK_ABORT_ON_FAIL(result, "Could not create Render Semaphore");
		m_DeletionQueue.PushBack(RenderSemaphore);

		//
		// Init Pipeline
		//
		VkShaderModule triangleFragShader;
		VkShaderModule triangleVertShader;
		VkShaderModule redTraingleFragShader;
		VkShaderModule redTriangleVertShader;
		VkShaderModule meshTriangleVertShader;
		bool bResult = VulkanFactory::CreateShaderFromFile(Device, "color_triangle.frag.spv", &triangleFragShader);
		ENGINE_ASSERT(bResult, "Could not load Color Traingle Fragment Shader");

		bResult = VulkanFactory::CreateShaderFromFile(Device, "color_triangle.vert.spv", &triangleVertShader);
		ENGINE_ASSERT(bResult, "Could not load Color Traingle Vertex Shader");

		bResult = VulkanFactory::CreateShaderFromFile(Device, "red_triangle.frag.spv", &redTraingleFragShader);
		ENGINE_ASSERT(bResult, "Could not load Red Traingle Vertex Shader");		
		
		bResult = VulkanFactory::CreateShaderFromFile(Device, "red_triangle.vert.spv", &redTriangleVertShader);
		ENGINE_ASSERT(bResult, "Could not load Red Traingle Vertex Shader");

		bResult = VulkanFactory::CreateShaderFromFile(Device, "triangle_mesh.vert.spv", &meshTriangleVertShader);
		ENGINE_ASSERT(bResult, "Could not load Triangle Mesh Vertex Shader");

		result = VulkanFactory::CreatePipelineLayout(Device, &TrianglePipelineLayout);
		VK_ABORT_ON_FAIL(result, "Could not create Pipeline Layout");
		m_DeletionQueue.PushBack(TrianglePipelineLayout);

		VulkanPipelineBuilder pipelineBuilder;
		pipelineBuilder.ShaderStages.push_back(VulkanFactory::ShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, triangleVertShader));
		pipelineBuilder.ShaderStages.push_back(VulkanFactory::ShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, triangleFragShader));
		pipelineBuilder.VertexInputState = VulkanFactory::VertexInputStateCreateInfo();
		pipelineBuilder.InputAssemblyState = VulkanFactory::InputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
		pipelineBuilder.Viewport.x = 0.0f;
		pipelineBuilder.Viewport.y = 0.0f;
		pipelineBuilder.Viewport.width = (float)SwapchainExtend.width;
		pipelineBuilder.Viewport.height = (float)SwapchainExtend.height;
		pipelineBuilder.Viewport.minDepth = 0.0f;
		pipelineBuilder.Viewport.maxDepth = 1.0f;
		pipelineBuilder.Scissor.offset = { 0,0 };
		pipelineBuilder.Scissor.extent = SwapchainExtend;
		pipelineBuilder.RasterizarionState = VulkanFactory::RastzerizationStateCreateInfo(VK_POLYGON_MODE_FILL);
		pipelineBuilder.MultisamnpleSate = VulkanFactory::MultisampleSateCreateInfo();
		pipelineBuilder.ColorBlendAttachment = VulkanFactory::ColorBlendAttachmentState();
		pipelineBuilder.PipelineLayout = TrianglePipelineLayout;

		TrianglePipeline = pipelineBuilder.BuildPipeline(Device, RenderPass);
		m_DeletionQueue.PushBack(TrianglePipeline);

		pipelineBuilder.ShaderStages.clear();
		pipelineBuilder.ShaderStages.push_back(VulkanFactory::ShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, redTriangleVertShader));
		pipelineBuilder.ShaderStages.push_back(VulkanFactory::ShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, redTraingleFragShader));
		
		RedTrianglePipeline = pipelineBuilder.BuildPipeline(Device, RenderPass);
		m_DeletionQueue.PushBack(RedTrianglePipeline);

		pipelineBuilder.ShaderStages.clear();
		pipelineBuilder.ShaderStages.push_back(VulkanFactory::ShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, meshTriangleVertShader));
		pipelineBuilder.ShaderStages.push_back(VulkanFactory::ShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, triangleFragShader));

		VertexInputDescription vertexDesc = Vertex::GetVertexDescription();
		pipelineBuilder.VertexInputState.pVertexAttributeDescriptions = vertexDesc.Attributes.data();
		pipelineBuilder.VertexInputState.vertexAttributeDescriptionCount = vertexDesc.Attributes.size();
		pipelineBuilder.VertexInputState.pVertexBindingDescriptions = vertexDesc.Bidnings.data();
		pipelineBuilder.VertexInputState.vertexBindingDescriptionCount = vertexDesc.Bidnings.size();
		pipelineBuilder.VertexInputState.flags = vertexDesc.Flags;

		MeshPipeline = pipelineBuilder.BuildPipeline(Device, RenderPass);
		m_DeletionQueue.PushBack(MeshPipeline);

		vkDestroyShaderModule(Device, triangleFragShader, nullptr);
		vkDestroyShaderModule(Device, triangleVertShader, nullptr);
		vkDestroyShaderModule(Device, redTraingleFragShader, nullptr);
		vkDestroyShaderModule(Device, redTriangleVertShader, nullptr);
		vkDestroyShaderModule(Device, meshTriangleVertShader, nullptr);


		VmaAllocatorCreateInfo allocatorInfo{};
		allocatorInfo.physicalDevice = PhysicalDevice;
		allocatorInfo.device = Device;
		allocatorInfo.instance = Instance;
		vmaCreateAllocator(&allocatorInfo, &m_Allocator);

		LoadMeshes();
	}

	void VulkanContext::LoadMeshes() {
		TriangleMesh.Vertecies.resize(3);
		TriangleMesh.Vertecies[0].Position = { 1.0f, 1.0f, 0.0f };
		TriangleMesh.Vertecies[1].Position = { -1.0f, 1.0f, 0.0f };
		TriangleMesh.Vertecies[2].Position = { 0.0f, -1.0f, 0.0f };
		TriangleMesh.Vertecies[0].Color = { 0.0f, 1.0f, 0.0f };
		TriangleMesh.Vertecies[1].Color = { 0.0f, 1.0f, 0.0f };
		TriangleMesh.Vertecies[2].Color = { 0.0f, 1.0f, 0.0f };

		UploadMesh(TriangleMesh);
	}

	void VulkanContext::UploadMesh(Mesh& mesh) {
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.pNext = nullptr;
		bufferInfo.size = mesh.Vertecies.size() * sizeof(Vertex);
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

		VmaAllocationCreateInfo vmaallocInfo{};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

		VkResult result = vmaCreateBuffer(m_Allocator, &bufferInfo, &vmaallocInfo, &mesh.VertexBuffer.Buffer, &mesh.VertexBuffer.Allocation, nullptr);
		VK_ABORT_ON_FAIL(result, "Could not upload mesh to gpu");

		void* data;
		vmaMapMemory(m_Allocator, mesh.VertexBuffer.Allocation, &data);
		memcpy(data, mesh.Vertecies.data(), mesh.Vertecies.size() * sizeof(Vertex));
		vmaUnmapMemory(m_Allocator, mesh.VertexBuffer.Allocation);

		//TODO delete buffer later
	}

	VulkanContext::~VulkanContext() {
		m_DeletionQueue.DeleteAll();

		vkDestroyDevice(Device, nullptr);
		vkb::destroy_debug_utils_messenger(Instance, DebugMessenger, nullptr);
		vkDestroyInstance(Instance, nullptr);
	}
}