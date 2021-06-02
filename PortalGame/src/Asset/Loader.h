#pragma once

#include <string>
#include <memory>

#include "OpenGL/Shader.h"

namespace PGame {
	namespace Asset {
		class Loader {
		public:
			Loader();

			bool Load(const std::string& path, std::shared_ptr<GL::Shader> shader);
			bool LoadText(const std::string& path, std::string& contents);
		};
	}
}

