#pragma once
#include "Graphics/VulkanContext.h"

namespace Engine {
	class Renderer {
	public:
		Renderer();
		
		void Initialize();
		void DrawFrame();

		void WaitForGPUIdle();

		bool SelectedPipeline = false;

	private:
		VulkanContext m_VkContext;

		int m_FrameNumber;
	};
}