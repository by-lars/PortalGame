#pragma once

#include <string>
#include <memory>
#include <filesystem>

#include "OpenGL/Shader.h"
#include "Renderer/Data.h"

namespace Engine {
	namespace Asset {
			bool Load(const std::filesystem::path& path, std::shared_ptr<GL::Shader> outShader);
			void LoadOBJ(const std::filesystem::path& file, std::vector<Renderer::Vertex>& outVerticies);
			bool LoadText(const std::filesystem::path& path, std::string& contents);
	}
}

