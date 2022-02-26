#pragma once
#include <string>

namespace Engine {
	struct Settings { 
		struct {
			const std::string Name = "No Name";
		} Engine;

		struct {
			const bool EnableValidationLayers = true;
		} Vulkan;

		struct {
			std::string Name;
		} App;
	};
}