#pragma once

#include <string>
#include <memory>
#include <filesystem>

#include "OpenGL/Shader.h"
#include "Renderer/Data.h"

namespace fs = std::filesystem;

namespace Engine {
	namespace Asset {
			template<typename T>
			bool Load(const fs::path& path, std::shared_ptr<T>);
			
			template<>
			bool Load<GL::Shader>(const fs::path& path, std::shared_ptr<GL::Shader> outShader);

			template<>
			bool Load<Renderer::Mesh>(const fs::path& file, std::shared_ptr<Renderer::Mesh> outMesh);

			bool LoadText(const fs::path& path, std::string& contents);
	}
}

