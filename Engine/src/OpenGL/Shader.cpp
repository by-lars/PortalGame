#include "OpenGL/Shader.h"
#include "Core/Base.h"
#include "Util/StringHelper.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Engine {
	namespace GL {
		static const std::string NAME_TOKEN = "#pragma name ";
		static const std::string TYPE_TOKEN = "#pragma type ";

		static GLenum GetShaderType(const std::string& type) {
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment")
				return GL_FRAGMENT_SHADER;

			pgError("Unkown shader type: '" << type << "'");
			return PG_FAILURE;
		}

		Shader::Shader() {
			m_Id = 0;
		}

		Shader::~Shader() {
			glDeleteProgram(m_Id);
			pgDebug("Deleted shader id=" << m_Id);
		}

		void Shader::Use() {
			glUseProgram(m_Id);
		}

		GLuint Shader::GetId() {
			return m_Id;
		}

		void Shader::SetMatrix(const char* name, const glm::mat4& matrix) {
			Use();
			glUniformMatrix4fv(glGetUniformLocation(m_Id, name), 1, GL_FALSE, glm::value_ptr(matrix));
		}


		bool Shader::Load(const std::string& source) {
			if (m_Id != 0) {
				glDeleteProgram(m_Id);
			}

			m_Id = glCreateProgram();
			pgDebug("Created shader id=" << m_Id);

			std::string name = SHelper::GetBetween(source, NAME_TOKEN, "\n");

			if (source.empty()) {
				pgError("Can't compile empty Shader File");
				return PG_FAILURE;
			}

			pgInfo("Starting to compile Shader '" << name << "'");

			size_t pos = source.find(TYPE_TOKEN);
			while (pos != std::string::npos) {
				//Step 1: Get the type of  SubShader
				size_t begin = TYPE_TOKEN.length() + pos;
				size_t end = source.find('\n', pos);

				//If type directive is at end there isn't any shader code
				if (end == std::string::npos) {
					pgError("Shader type directive can't be at end of file");
					return PG_FAILURE;
				}

				//Convert type from string to GLeunm
				std::string subShaderTypeStr = source.substr(begin, end - begin);
				GLenum subShaderType = GetShaderType(subShaderTypeStr);
				if (subShaderType == PG_FAILURE) { return PG_FAILURE; }

				//Step 2: Get source of SubShader
				//pos is either at the end of the file or at another shader
				pos = source.find(TYPE_TOKEN, end);

				std::string subShaderSource = (
					pos == std::string::npos
					? source.substr(end, source.length() - end) // Shader is last in file
					: source.substr(end, pos - end) // Shader is not last in file
					);

				//Step 3: Compile SubShader
				int subShaderId = Compile(subShaderType, subShaderSource.c_str());

				if (subShaderId == PG_FAILURE) {
					pgError("Failed to compile '" << subShaderTypeStr << "' shader of '" << name << "'");
					return PG_FAILURE;
				}

				glAttachShader(m_Id, subShaderId);
				pgInfo("Compiled '" << subShaderTypeStr << "' shader of '" << name << "'");

			}

			//Step 5: link shaders
			if (Link() == PG_FAILURE) {
				pgError("Failed to link shader program '" << name << "'");
				//TODO: cleanup shaders like frag and vert
				return PG_FAILURE;
			}

			//Step 6: Cleanup
			//TODO: Do cleanup

			return PG_SUCCESS;
		}

		int Shader::Compile(GLenum shaderType, const char* source) {
			GLuint shaderId = glCreateShader(shaderType);
			glShaderSource(shaderId, 1, &source, NULL);
			glCompileShader(shaderId);

			//TODO: Change char[] into something better like an STL container.
			char infoLog[1024];
			if (Check(shaderId, true, infoLog, sizeof(infoLog)) == PG_FAILURE) {
				pgError("Compile Log: " << std::endl << infoLog);
				return PG_FAILURE;
			}

			return shaderId;
		}

		bool Shader::Link() {
			glLinkProgram(m_Id);

			char infoLog[1024];
			if (Check(m_Id, false, infoLog, sizeof(infoLog)) == PG_FAILURE) {
				//Compile failed
				pgError("Link Log: " << std::endl << infoLog);

				return PG_FAILURE;
			}

			return PG_SUCCESS;
		}

		bool Shader::Check(GLuint sid, bool isShader, char* infoLog, GLuint logSize) {
			int succ;

			//Check using a different function, if it's not a shader
			if (isShader) {
				glGetShaderiv(sid, GL_COMPILE_STATUS, &succ);
				if (!succ) {
					glGetShaderInfoLog(sid, logSize, NULL, infoLog);
					return PG_FAILURE;
				}

			}
			else {
				glGetProgramiv(sid, GL_LINK_STATUS, &succ);
				if (!succ) {
					glGetProgramInfoLog(sid, logSize, NULL, infoLog);
					return PG_FAILURE;
				}
			}

			return PG_SUCCESS;
		}
	}
}