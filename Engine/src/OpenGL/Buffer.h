#pragma once

#include <glad/glad.h>
#include <cstdint>
#include <initializer_list>

#include "OpenGL/BufferElement.h"

namespace Engine {
	namespace GL {
		enum class BufferTypes {
			ARRAY_BUFFER					= GL_ARRAY_BUFFER,
			ATOMIC_COUNTER_BUFFER			= GL_ATOMIC_COUNTER_BUFFER,
			COPY_READ_BUFFER				= GL_COPY_READ_BUFFER,
			COPY_WRITE_BUFFER				= GL_COPY_WRITE_BUFFER,
			//DISPATCH_INDIRECT_BUFFERFER		= GL_DISPATCH_INDIRECT_BUFFERFER,
			DRAW_INDIRECT_BUFFER			= GL_DRAW_INDIRECT_BUFFER,
			ELEMENT_ARRAY_BUFFER			= GL_ELEMENT_ARRAY_BUFFER,
			PIXEL_PACK_BUFFER				= GL_PIXEL_PACK_BUFFER,
			PIXEL_UNPACK_BUFFER				= GL_PIXEL_UNPACK_BUFFER,
			QUERY_BUFFER					= GL_QUERY_BUFFER,
			SHADER_STORAGE_BUFFER			= GL_SHADER_STORAGE_BUFFER,
			TEXTURE_BUFFER					= GL_TEXTURE_BUFFER,
			//TRANSFORM_FEEDBACK_BUFFERFFER	= GL_TRANSFORM_FEEDBACK_BUFFERFFER,
			UNIFORM_BUFFER					= GL_UNIFORM_BUFFER
		};

		enum class BufferUsages {
			STATIC_DRAW = GL_STATIC_DRAW,
			STATIC_READ = GL_STATIC_READ,
			STATIC_COPY = GL_STATIC_COPY,

			STREAM_DRAW = GL_STREAM_DRAW,
			STREAM_READ = GL_STREAM_READ,
			STREAM_COPY = GL_STREAM_COPY,

			DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
			DYNAMIC_READ = GL_DYNAMIC_READ,
			DYNAMIC_COPY = GL_DYNAMIC_COPY
		};

		class Buffer {
		public:
			Buffer() = default;
			~Buffer();

			void Init(BufferTypes type, BufferUsages usage, uint32_t size);
			void Init(BufferTypes type, BufferUsages usage, uint32_t size, GLuint vaoId);

			bool Upload(void* data, uint32_t offset, uint32_t size);
			bool PushBack(void* data, uint32_t size);

			void SetFormat(uint32_t startAttrib, const std::initializer_list<BufferElement>& format);
			void SetFormat(const std::initializer_list<BufferElement>& format);


			GLuint GetCurrentOffsetBytes();
			GLuint GetCurrentOffsetIndex();

			GLuint GetVAO();

			void Bind();
			void Unbind();

		private:
			BufferTypes m_Type;
			uint32_t m_Size;
			uint32_t m_Stride;
			uint32_t m_CurrentOffset;
			GLuint m_BufferId;
			GLuint m_VAOid;
		};
	}
}