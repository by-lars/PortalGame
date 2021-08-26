#pragma once

#include <glad/glad.h>
#include "OpenGL/Types.h"
#include <string>


namespace Engine {
	namespace GL {
		enum class TexType {
			TEX_1D						= GL_TEXTURE_1D,
			TEX_2D						= GL_TEXTURE_2D,
			TEX_3D						= GL_TEXTURE_3D,
			TEX_1D_ARRAY				= GL_TEXTURE_1D_ARRAY,
			TEX_2D_ARRAY				= GL_TEXTURE_2D_ARRAY,
			TEX_RECTANGLE				= GL_TEXTURE_RECTANGLE,
			TEX_CUBE_MAP				= GL_TEXTURE_CUBE_MAP,
			TEX_CUBE_MAP_ARRAY			= GL_TEXTURE_CUBE_MAP,
			TEX_BUFFER					= GL_TEXTURE_BUFFER,
			TEX_2D_MULTISAMPLE			= GL_TEXTURE_2D_MULTISAMPLE,
			TEX_2D_MULTISAMPLE_ARRAY	= GL_TEXTURE_2D_MULTISAMPLE_ARRAY
		};

		enum class TexUnit {
			U0 = GL_TEXTURE0,
			U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12,
			U13, U14, U15, U16, U17, U18, U19, U20, U21, U22,
			U23, U24, U25, U26, U27, U28, U29, U30, U31
		};

		enum class TexWrapping {
			REPEAT = GL_REPEAT,
			MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
			CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
			CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
		};

		enum class TexFiltering {
			NEAREST = GL_NEAREST,
			LINEAR = GL_LINEAR,
		};

		class Texture {
		public:
			Texture();
			~Texture();

			void Init(TexType type, TexUnit unit, TexWrapping wrapping, TexFiltering filtering);

			void Upload(GLuint width, GLenum format, GLenum internalFormat, const GLubyte* data);
			void Upload(GLuint width, GLuint height, GLenum format, GLenum internalFormat, const GLubyte* data);
			void Upload(GLuint width, GLuint height, GLuint depth, GLenum format, GLenum internalFormat, const GLubyte* data);

			void SetUnit(GLuint unit);
			GLuint GetUnitNumber();

			void Bind();
			void UnBind();
		private:
			GLuint m_Id;
			TexUnit m_Unit;
			TexType m_Type;
		};
	}
}