#include "Asset/Loader.h"

#include "Core/Base.h"

#include <string>
#include <fstream>
#include <memory>

namespace PGame {
	namespace Asset {
		Loader::Loader() {

		}

		bool Loader::Load(const std::string& path, std::shared_ptr<GL::Shader> shader) {
			std::string shaderSource;
			if (LoadText(path, shaderSource) == PG_FAILURE) { return PG_FAILURE; }
			return shader->Load(shaderSource);
		}

		bool Loader::LoadText(const std::string& path, std::string& contents) {
			std::ifstream file(path);

			if (!file) {
				pgError("Could not open file -> " << path);
				return PG_FAILURE;
			}

			file.seekg(0, std::ios::end);
			size_t size = file.tellg();

			contents = std::string(size, ' ');

			file.seekg(0, std::ios::beg);
			file.read(&contents[0], size);

			return PG_SUCCESS;
		}

	}
}