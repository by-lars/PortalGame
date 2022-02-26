#include "Renderer.h"
#include "Graphics/VulkanHelper.h"

namespace Engine {
	Renderer::Renderer() {
	
	}

	void Renderer::Initialize() {
		m_VkContext.Initialize();
	}

	void Renderer::WaitForGPUIdle() {
		vkDeviceWaitIdle(m_VkContext.Device);
	}

	void Renderer::DrawFrame() {
		VK_ABORT_ON_FAIL(vkWaitForFences(m_VkContext.Device, 1, &m_VkContext.RenderFence, true, 1000000000), "GPU Timed Out");
		VK_ABORT_ON_FAIL(vkResetFences(m_VkContext.Device, 1, &m_VkContext.RenderFence), "GPU Timed Out");

		uint32_t swapchainImageIndex;
		VK_ABORT_ON_FAIL(vkAcquireNextImageKHR(m_VkContext.Device, m_VkContext.Swapchain, 1000000000, m_VkContext.PresentSemaphore, nullptr, &swapchainImageIndex), "Could not get next image");

		VK_ABORT_ON_FAIL(vkResetCommandBuffer(m_VkContext.CommandBuffer, 0), "Could not reset command buffer");
		
		VkCommandBuffer cmd = m_VkContext.CommandBuffer;
		VkCommandBufferBeginInfo cmdBeginInfo = {};
		cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBeginInfo.pNext = nullptr;
		cmdBeginInfo.pInheritanceInfo = nullptr;
		cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VK_ABORT_ON_FAIL(vkBeginCommandBuffer(cmd, &cmdBeginInfo), "Could not being command buffer");

		VkClearValue clearValue;
		float flash = abs(sin(m_FrameNumber / 120.0f));
		clearValue.color = { {0.0f, 0.0f, flash, 1.0f} };

		VkRenderPassBeginInfo rpInfo{};
		rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		rpInfo.pNext = nullptr;
		rpInfo.renderPass = m_VkContext.RenderPass;
		rpInfo.renderArea.offset.x = 0;
		rpInfo.renderArea.offset.y = 0;
		rpInfo.renderArea.extent = m_VkContext.SwapchainExtend;
		rpInfo.framebuffer = m_VkContext.FrameBuffers[swapchainImageIndex];
		rpInfo.clearValueCount = 1;
		rpInfo.pClearValues = &clearValue;

		vkCmdBeginRenderPass(cmd, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdEndRenderPass(cmd);
		VK_ABORT_ON_FAIL(vkEndCommandBuffer(cmd), "Could not end Command Buffer");


		VkSubmitInfo submit{};
		submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit.pNext = nullptr;

		VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		submit.pWaitDstStageMask = &waitStage;
		submit.waitSemaphoreCount = 1;
		submit.pWaitSemaphores = &m_VkContext.PresentSemaphore;
		submit.signalSemaphoreCount = 1;
		submit.pSignalSemaphores = &m_VkContext.RenderSemaphore;
		submit.commandBufferCount = 1;
		submit.pCommandBuffers = &cmd;

		VK_ABORT_ON_FAIL(vkQueueSubmit(m_VkContext.GrphicsQueue, 1, &submit, m_VkContext.RenderFence), "Could not submite render queue");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = nullptr;
		presentInfo.pSwapchains = &m_VkContext.Swapchain;
		presentInfo.swapchainCount = 1;
		presentInfo.pWaitSemaphores = &m_VkContext.RenderSemaphore;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pImageIndices = &swapchainImageIndex;

		VK_ABORT_ON_FAIL(vkQueuePresentKHR(m_VkContext.GrphicsQueue, &presentInfo), "Could not submit present queue");

		m_FrameNumber++;
	}
}