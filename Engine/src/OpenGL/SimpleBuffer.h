#pragma once

#include "OpenGL/Types.h"
#include "OpenGL/BufferElement.h"

#include <stdint.h>
#include <initializer_list>

namespace Engine {
	namespace GL {
		class SimpleBuffer {
		public:
			SimpleBuffer(uint32_t size);
			~SimpleBuffer();

			void Upload(void* verts, uint32_t size) const;
			void SetFormat(std::initializer_list<BufferElement> format) const;

			void Bind() const;
			void Unbind() const;

		private:
			GLuint m_Buffer;
			GLuint m_VAO;
		};
	}
}
