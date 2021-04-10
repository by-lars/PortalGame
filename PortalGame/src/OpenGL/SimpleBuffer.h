#pragma once

#include "Util/Types.h"

#include <stdint.h>
#include <initializer_list>

namespace PGame {
	namespace GL {
		class BufferElement {
		public:
			BufferElement(GLenum type, uint32_t count);

			uint32_t GetSize() const;
			bool IsDecimalType() const;

			GLenum Type;
			uint32_t Count;
		};

		class SimpleBuffer {
		public:
			SimpleBuffer(uint32_t size);
			~SimpleBuffer();

			void Upload(float* verts, uint32_t size) const;
			void SetFormat(std::initializer_list<BufferElement> format) const;

			void Bind() const;
			void Unbind() const;

		private:
			GLuint m_Buffer;
			GLuint m_VAO;
		};
	}
}
