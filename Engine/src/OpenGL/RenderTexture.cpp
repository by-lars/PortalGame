#include <glad/glad.h>

#include "OpenGL/RenderTexture.h"
#include "Core/Base.h"

namespace Engine {
	namespace GL {
		RenderTexture::RenderTexture(GLuint width, GLuint height) 
			: m_Width(width), m_Height(height) {

			glGenFramebuffers(1, &m_Id);
			glBindFramebuffer(GL_FRAMEBUFFER, m_Id);

			glGenTextures(1, &m_TextureId);
			glBindTexture(GL_TEXTURE_2D, m_TextureId);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureId, 0);

			assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
				
			pgDebug("Created Framebuffer id=" << m_Id);
			pgDebug("Created Texture id=" << m_TextureId);
		}

		RenderTexture::~RenderTexture() {
			glDeleteFramebuffers(1, &m_Id);
			pgDebug("Deleted Framebuffer id=" << m_Id);
			pgDebug("Deleted Texture id=" << m_TextureId);
		}

		GLuint RenderTexture::GetBufferId() {
			return m_Id;
		}

		GLuint RenderTexture::GetTextureId() {
			return m_TextureId;
		}

		void RenderTexture::Bind() {
			glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
		}

		void RenderTexture::UnBind() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}