#include "OpenGL/Buffer.h"
#include "Core/Base.h"

#include <glm/glm.hpp>

namespace PGame {
	namespace GL {
		/*
			Buffer Element
		*/

		BufferElement::BufferElement(DataTypes type, uint32_t count) :
			Type(type), Count(count), Divisor(0) {}

		BufferElement::BufferElement(DataTypes type, uint32_t count, uint32_t divisor) :
			Type(type), Count(count), Divisor(divisor) {}

		uint32_t BufferElement::GetSize() const {
			switch (Type) {
				case DataTypes::FLOAT:	return Count * sizeof(GLfloat);
				case DataTypes::INT:	return Count * sizeof(int32_t);
				case DataTypes::U_INT:	return Count * sizeof(uint32_t);
				case DataTypes::DOUBLE:	return Count * sizeof(GLdouble);
				case DataTypes::MAT4f:	return Count * sizeof(glm::mat4);
			}

			pgAssert(false, "Can't determine size for unknown Buffer Element Type: " << (int)Type)
			return 0;
		}

		bool BufferElement::IsDecimalType() const {
			switch (Type) {
				case DataTypes::BYTE:
				case DataTypes::SHORT:
				case DataTypes::INT:
				case DataTypes::U_BYTE:
				case DataTypes::U_SHORT:
				case DataTypes::U_INT:
					return false;

				case DataTypes::DOUBLE:
				case DataTypes::HALF_FLOAT:
				case DataTypes::FLOAT:
				case DataTypes::MAT4f:
					return true;
			}

			pgAssert(false, "Can't determine category for unknown Buffer Element Type: " << (int)Type);
			return 0;
		}



		/*
			Buffer
		*/

		void Buffer::Init(BufferTypes type, BufferUsages usage, uint32_t size) {
			m_Type = type;
			m_Size = size;
			
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

		//Probably should not use this functions
		//as it could corrupt other data used by PushBack
		//Probably only should be using this to manipulate data
		//Which has already been uploaded by PushBack
		void Buffer::Upload(void* data, uint32_t offset, uint32_t size) {
			Bind();
			glBufferSubData((GLenum)m_Type, offset, size, data);

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

			GLsizei stride = 0;

			//Determine stride for one Vertex
			for (const auto& element : format) {
				stride += element.GetSize();
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
						glVertexAttribPointer(attrId, element.Count, (GLenum)element.Type, GL_FALSE, stride, (const void*)offset);
					}
					else {
						glVertexAttribIPointer(attrId, element.Count, (GLenum)element.Type, stride, (const void*)offset);
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

	}
}