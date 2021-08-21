#pragma once

#include <glad/glad.h>
#include <cstdint>

namespace Engine {
	namespace GL {
		enum class DataTypes {
			MAT4f = 0,

			DOUBLE = GL_DOUBLE,
			FLOAT = GL_FLOAT,
			HALF_FLOAT = GL_HALF_FLOAT,
			INT = GL_INT,
			U_INT = GL_UNSIGNED_INT,
			BYTE = GL_BYTE,
			U_BYTE = GL_UNSIGNED_BYTE,
			SHORT = GL_SHORT,
			U_SHORT = GL_UNSIGNED_SHORT,
		};

		struct BufferElement {
			BufferElement(DataTypes type, uint32_t count);
			BufferElement(DataTypes type, uint32_t count, uint32_t divisior);

			uint32_t GetSize() const;
			bool IsDecimalType() const;

			DataTypes Type;
			uint32_t Count;
			uint32_t Divisor;
		};
	}
}