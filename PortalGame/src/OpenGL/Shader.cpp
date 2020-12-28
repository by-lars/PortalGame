#include "OpenGL/Shader.h"
#include "Util/Defines.h"
#include "Util/StringHelper.h"

#include <glad/glad.h>

namespace PGame {
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

	GL::Shader::Shader() {
		id = glCreateProgram();
		pgDebug("Created shader id=" << id);
	}

	GL::Shader::~Shader() {
		glDeleteProgram(id);
		pgDebug("Deleted shader id=" << id);
	}

	void GL::Shader::Use() {
		glUseProgram(id);
	}

	bool GL::Shader::Load(const std::string& source) {
		std::string name = SHelper::GetBetween(source, NAME_TOKEN, "\n");
		//TODO: write string helper function to improve loop below

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
			if (subShaderType == PG_FAILURE) {return PG_FAILURE;}

			//Step 2: Get source of SubShader
			//pos is either at the end of the file or at another shader
			pos = source.find(TYPE_TOKEN, end);

			std::string subShaderSource = (
			pos == std::string::npos 
				? source.substr(end, source.length() - end) // Shader is last in file
				: source.substr(end, pos - end			  ) // Shader is not last in file
			); 

			//Step 3: Compile SubShader
			int subShaderId = Compile(subShaderType, subShaderSource.c_str());

			if (subShaderId == PG_FAILURE) {
				pgError("Failed to compile '" << subShaderTypeStr << "' shader of '" << name << "'");
				return PG_FAILURE;
			}

			glAttachShader(id, subShaderId);
			pgInfo("Compiled '" << subShaderTypeStr << "' shader of '" << name << "'");

		}

		//Step 5: link shaders
		if (Link() == PG_FAILURE) {
			pgInfo("Failed to link shader program '" << name << "'");
			//TODO: cleanup shaders like frag and vert
			return PG_FAILURE;
		}

		//Step 6: Cleanup
		//TODO: Do cleanup

		return PG_SUCCESS;
	}

	int GL::Shader::Compile(GLenum shaderType, const char* source) {
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

	bool GL::Shader::Link() {
		glLinkProgram(id);

		char infoLog[1024];
		if (Check(id, false, infoLog, sizeof(infoLog)) == PG_FAILURE) {
			//Compile failed
			pgError("Link Log Log: " << std::endl << infoLog);

			return PG_FAILURE;
		}

		return PG_SUCCESS;
	}

	bool GL::Shader::Check(GLuint id, bool isShader, char* infoLog, GLuint logSize) {
		int succ;

		//Check using a different function, if it's not a shader
		if (isShader) {
			glGetShaderiv(id, GL_COMPILE_STATUS, &succ);
			if (!succ) {
				glGetShaderInfoLog(id, logSize, NULL, infoLog);
				return PG_FAILURE;
			}

		}
		else {
			glGetProgramiv(id, GL_LINK_STATUS, &succ);
			if (!succ) {
				glGetProgramInfoLog(id, logSize, NULL, infoLog);
				return PG_FAILURE;
			}
		}

		return PG_SUCCESS;
	}
}