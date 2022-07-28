#pragma once
#include "Graphics/VulkanHelper.h"

#include<vector>
#include <vulkan/vulkan.h>


namespace Engine {
	class VkDeletionQueue {
	private:
		typedef struct Handle_T* Handle;
		struct QueueItem {
			VkObjectType Type;
			Handle Handle;
		};

	public:
		void Initialize(VkInstance instance, VkDevice device);

		template<typename T>
		void PushBack(T handle) {
			m_Objects.push_back(QueueItem{
					GetVulkanType<T>(),
					(Handle)handle
				});
		}
	
		void DeleteAll();
	private:
		VkDevice m_Device;
		VkInstance m_Instance;
		std::vector<QueueItem> m_Objects;
	};
}