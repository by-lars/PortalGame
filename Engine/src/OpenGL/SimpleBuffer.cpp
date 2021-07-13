#include "OpenGL/SimpleBuffer.h"
#include "Core/Base.h"

#include <glad/glad.h>

namespace Engine {
	namespace GL {
		BufferElement::BufferElement(GLenum type, uint32_t count) :
			Type(type), Count(count) {}

		uint32_t BufferElement::GetSize() const {
			switch (Type)
			{
			case GL_FLOAT:			return Count * sizeof(GLfloat);
			case GL_INT:			return Count * sizeof(int32_t);
			case GL_UNSIGNED_INT:	return Count * sizeof(uint32_t);
			case GL_DOUBLE:			return Count * sizeof(GLdouble);
			}

			pgError("Can't determine size for unknown Buffer Element Type: " << Type);
			return 0;
		}

		bool BufferElement::IsDecimalType() const {
			switch (Type) {
			case GL_BYTE:
			case GL_SHORT:
			case GL_INT:
			case GL_UNSIGNED_BYTE:
			case GL_UNSIGNED_SHORT:
			case GL_UNSIGNED_INT:
				return false;

			case GL_DOUBLE:
			case GL_HALF_FLOAT:
			case GL_FLOAT:
				return true;
			}

			pgError("Can't determine category for unknown Buffer Element Type: " << Type);
			return 0;
		}

		SimpleBuffer::SimpleBuffer(uint32_t size) {
			glGenVertexArrays(1, &m_VAO);
			glBindVertexArray(m_VAO);
			pgDebug("Created VAO id=" << m_VAO);

			glGenBuffers(1, &m_Buffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
			glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
			pgDebug("Created VBO id=" << m_Buffer);
		}

		SimpleBuffer::~SimpleBuffer() {
			glDeleteVertexArrays(1, &m_VAO);
			pgDebug("Deleted VAO id=" << m_VAO);

			glDeleteBuffers(1, &m_Buffer);
			pgDebug("Deleted VBO id=" << m_Buffer);
		}

		void SimpleBuffer::Bind() const {
			glBindVertexArray(m_VAO);
		}

		void SimpleBuffer::Unbind() const {
			glBindVertexArray(0);
		}

		void SimpleBuffer::Upload(void* verts, uint32_t size) const {
			glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, size, verts);
		}

		void SimpleBuffer::SetFormat(std::initializer_list<BufferElement> format) const {
			Bind();

			GLsizei stride = 0;

			//Determine stride for one Vertex
			for (const auto& element : format) {
				stride += element.GetSize();
			}

			//Setup attribute pointers
			GLuint  attrId = 0;
			size_t offset = 0;
			for (const auto& element : format) {
				glEnableVertexAttribArray(attrId);

				if (element.IsDecimalType()) {
					glVertexAttribPointer(attrId, element.Count, element.Type, GL_FALSE, stride, (const void*)offset);
				}
				else {
					glVertexAttribIPointer(attrId, element.Count, element.Type, stride, (const void*)offset);
				}

				offset += element.GetSize();
				attrId += 1;
			}

			Unbind();
		}
	}
}