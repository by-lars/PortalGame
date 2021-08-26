#include "OpenGL/Texture.h"
#include "Core/Base.h"
#include <glad/glad.h>

namespace Engine {
	namespace GL {
		Texture::Texture() {
			m_Id = 0;
			m_Unit = TexUnit::U0;
			m_Type = TexType::TEX_2D;
		}

		Texture::~Texture() {
			pgDebug("Deleted Texture id=" << m_Id);
		}

		void Texture::Init(TexType type, TexUnit unit, TexWrapping wrapping, TexFiltering filtering) {
			m_Type = type;
			m_Unit = unit;

			glGenTextures(1, &m_Id);
			glActiveTexture((GLenum)m_Unit);
			glBindTexture((GLenum)m_Type, m_Id);

			glTexParameteri((GLenum)m_Type, GL_TEXTURE_WRAP_S, (GLenum)wrapping);
			glTexParameteri((GLenum)m_Type, GL_TEXTURE_WRAP_T, (GLenum)wrapping);
			glTexParameteri((GLenum)m_Type, GL_TEXTURE_WRAP_R, (GLenum)wrapping);

			glTexParameteri((GLenum)m_Type, GL_TEXTURE_MIN_FILTER, (GLenum)filtering);
			glTexParameteri((GLenum)m_Type, GL_TEXTURE_MAG_FILTER, (GLenum)filtering);
		
			pgDebug("Created Texture id=" << m_Id);
		}


		void Texture::Upload(GLuint width, GLenum format, GLenum internalFormat, const GLubyte* data) {
			Bind();
			glTexStorage1D((GLenum)m_Type, 1, internalFormat, width);
			glTexSubImage1D((GLenum)m_Type, 0, 0, width, format, GL_UNSIGNED_BYTE, (const void*)data);
		}

		void Texture::Upload(GLuint width, GLuint height, GLenum format, GLenum internalFormat, const GLubyte* data) {
			Bind();
			glTexStorage2D((GLenum)m_Type, 1, internalFormat, width, height);
			glTexSubImage2D((GLenum)m_Type, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, (const void*)data);
		}

		void Texture::Upload(GLuint width, GLuint height, GLuint depth, GLenum format, GLenum internalFormat, const GLubyte* data) {
			Bind();
			glTexStorage3D((GLenum)m_Type, 1, internalFormat, width, height, depth);
			glTexSubImage3D((GLenum)m_Type, 0, 0, 0, 0, width, height, depth, format, GL_UNSIGNED_BYTE, (const void*)data);
		}

		void Texture::SetUnit(GLuint unit) {
			glActiveTexture(unit);
			Bind();
		}

		GLuint Texture::GetUnitNumber() {
			return (GLuint)m_Unit - GL_TEXTURE0;
		}

		void Texture::Bind() {
			glBindTexture((GLenum)m_Type, m_Id);
		}

		void Texture::UnBind() {
			glBindTexture((GLenum)m_Type, 0);
		}
	}
}