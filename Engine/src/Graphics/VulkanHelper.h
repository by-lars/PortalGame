#pragma once

#include "Core/Base.h"
#include <vulkan/vulkan.h>
#include <stdlib.h>

namespace Engine {
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
}
