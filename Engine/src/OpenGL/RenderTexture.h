#pragma once

#include "OpenGL/Types.h"

namespace Engine {
	namespace GL {
		class RenderTexture {
		public:
			RenderTexture(GLuint width, GLuint height);
			~RenderTexture();

			GLuint GetBufferId();
			GLuint GetTextureId();

			void Bind();
			void UnBind();
		private:
			GLuint m_Id;
			GLuint m_TextureId;
			GLuint m_Width;
			GLuint m_Height;
		};
	}
}