#ifndef PG_OPENGL_SHADER_H
#define PG_OPENGL_SHADER_H

#include "Util/Types.h"

#include <string>
#include <unordered_map>

namespace PGame {
	namespace GL {
		class Shader {
		public:
			Shader();
			~Shader();

			bool Load(const std::string& source);
			void Use();

			GLuint id;
		private:
			int Compile(GLenum shaderType, const char* source);
			bool Link();
			bool Check(GLuint id, bool isShader, char* infoLog, GLuint logSize);
		};
	}
}

#endif