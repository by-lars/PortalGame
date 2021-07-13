#pragma once

#include "OpenGL/Types.h"

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Engine {
	namespace GL {
		class Shader {
		public:
			Shader();
			~Shader();

			bool Load(const std::string& source);
			void Use();

			GLuint GetId();

			void SetMatrix(const char* name, const glm::mat4& matrix);

		private:
			GLuint m_Id;
			int Compile(GLenum shaderType, const char* source);
			bool Link();
			bool Check(GLuint id, bool isShader, char* infoLog, GLuint logSize);
		};
	}
}
