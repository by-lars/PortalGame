#include "OpenGL/BufferElement.h"
#include "Core/Base.h"

#include <glm/glm.hpp>

namespace Engine {
	namespace GL {
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
	}
}