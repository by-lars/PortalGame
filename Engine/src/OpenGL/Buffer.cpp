#include "OpenGL/Buffer.h"
#include "Core/Base.h"
#include "OpenGL/Types.h"

#include <glm/glm.hpp>

namespace Engine {
	namespace GL {
		void Buffer::Init(BufferTypes type, BufferUsages usage, uint32_t size) {
			m_Type = type;
			m_Size = size;
			m_Stride = 0;
			
			//Make a new VAO if there is none
			glGenVertexArrays(1, &m_VAOid);
			glBindVertexArray(m_VAOid);
			
			//Create the acctual Buffer
			glGenBuffers(1, &m_BufferId);
			glBindBuffer((GLenum)m_Type, m_BufferId);
			glBufferData((GLenum)m_Type, m_Size, nullptr, (GLenum)usage);

			pgDebug("Created Buffer of " << size << " Bytes, id=" << m_BufferId);
			pgDebug("Created Vertex Array, id=" << m_VAOid);
		}

		void Buffer::Init(BufferTypes type, BufferUsages usage, uint32_t size, GLuint vaoId) {
			m_Type = type;
			m_Size = size;
			m_VAOid = vaoId;
			m_Stride = 0;

			//Use the "predefined" vao
			glBindVertexArray(m_VAOid);

			//Create the acctual Buffer
			glGenBuffers(1, &m_BufferId);
			glBindBuffer((GLenum)m_Type, m_BufferId);
			glBufferData((GLenum)m_Type, m_Size, nullptr, (GLenum)usage);

			pgDebug("Created Buffer of " << size << " Bytes, id=" << m_BufferId);
		}

		Buffer::~Buffer() {
			pgDebug("Deleted Buffer id=" << m_BufferId << " and VAO id=" << m_VAOid);
			glDeleteBuffers(1, &m_BufferId);
			glDeleteVertexArrays(1, &m_VAOid);
		}

		void Buffer::Upload(void* data, uint32_t offset, uint32_t size) {
			Bind();
			glBufferSubData((GLenum)m_Type, offset, size, data);

			//This is kinda dangerous
			if (offset + size > m_CurrentOffset) {
				m_CurrentOffset += size;
			}
		}

		bool Buffer::PushBack(void* data, uint32_t size)
		{
			if (size + m_CurrentOffset > m_Size) {
				pgError("Can't Upload Mesh to GPU, buffer too small");
				return false;
			}

			Bind();
			glBufferSubData((GLenum)m_Type, m_CurrentOffset, size, data);
			m_CurrentOffset += size;

			return true;
		}

		void Buffer::SetFormat(uint32_t attribOffset, const std::initializer_list<BufferElement>& format) {
			Bind();

			m_Stride = 0;
			//Determine stride for one Vertex
			for (const auto& element : format) {
				m_Stride += element.GetSize();
			}

			//Setup attribute pointers
			GLuint attrId = attribOffset;
			size_t offset = 0;
			for (const auto& element : format) {

				if (element.Type == DataTypes::MAT4f) {
					for (int i = 0; i < 4; i++) {
						glEnableVertexAttribArray(attrId + i);
						glVertexAttribPointer(attrId + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const void*)offset);
						glVertexAttribDivisor(attrId + i, element.Divisor);

						offset += 4 * sizeof(float);
					}
					attrId += 4;
				}
				else {
					glEnableVertexAttribArray(attrId);

					if (element.IsDecimalType()) {
						glVertexAttribPointer(attrId, element.Count, (GLenum)element.Type, GL_FALSE, m_Stride, (const void*)offset);
					}
					else {
						glVertexAttribIPointer(attrId, element.Count, (GLenum)element.Type, m_Stride, (const void*)offset);
					}


					attrId += 1;
					glVertexAttribDivisor(attrId, element.Divisor);
				}

				offset += element.GetSize();
			}
		}

		void Buffer::SetFormat(const std::initializer_list<BufferElement>& format) {
			SetFormat(0, format);
		}

		void Buffer::Bind()
		{
			glBindVertexArray(m_VAOid);
			glBindBuffer((GLenum)m_Type, m_BufferId);
		}

		void Buffer::Unbind()
		{
			glBindVertexArray(0);
			glBindBuffer((GLenum)m_Type, 0);
		}

		GLuint Buffer::GetCurrentOffsetBytes() {
			return m_CurrentOffset;
		}

		GLuint Buffer::GetCurrentOffsetIndex() {
			return m_CurrentOffset / m_Stride;
		}

		GLuint Buffer::GetVAO() {
			return m_VAOid;
		}
	}
}