#pragma once

#include <glm/glm.hpp>

namespace Engine {
	namespace Util {
		class Camera {
		public:
			enum class Direction
			{
				UP, DOWN, LEFT, RIGHT, FOREWARD, BACKWARD
			};

			Camera(glm::vec3 pos, glm::vec3 worldUp, float yaw, float pitch, float speed);

			void MoveDirection(Direction dir);
			void ProcessMouse(float x, float y);

			glm::mat4 GetViewMatrix();

		private:
			void UpdateVectors();

			glm::vec3 m_Pos, m_Front, m_Right, m_Up, m_WorldUp;
			float m_Speed, m_Pitch, m_Yaw;
		};
	}
}