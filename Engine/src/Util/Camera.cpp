#pragma once

#include "Util/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {
	namespace Util {
		Camera::Camera(glm::vec3 pos, glm::vec3 worldUp, float yaw, float pitch, float speed)
			: m_Pos(pos), m_WorldUp(worldUp), m_Yaw(yaw), m_Pitch(pitch), m_Speed(speed) {}

		void Camera::MoveDirection(Direction dir) {
			switch (dir)
			{
				case Direction::UP:
					m_Pos += m_WorldUp * m_Speed;
					break;
				case Direction::DOWN:
					m_Pos -= m_WorldUp * m_Speed;
					break;
				case Direction::LEFT:
					m_Pos -= m_Right * m_Speed;
					break;
				case Direction::RIGHT:
					m_Pos += m_Right * m_Speed;
					break;
				case Direction::FOREWARD:
					m_Pos += m_Front * m_Speed;
					break;
				case Direction::BACKWARD:
					m_Pos -= m_Front * m_Speed;
					break;
			}
		}

		void Camera::ProcessMouse(float x, float y) {
			static bool isFirstMouse = true;
			static float lastX = 0;
			static float lastY = 0;

			if (isFirstMouse) {
				lastX = x;
				lastY = y;
				isFirstMouse = false;
			}

			float xOff = x - lastX;
			float yOff = lastY - y;

			lastX = x;
			lastY = y;

			xOff *= 0.4f;
			yOff *= 0.4f;

			m_Yaw += xOff;
			m_Pitch += yOff;

			if (m_Pitch > 89.0f) {
				m_Pitch = 89.0f;
			}
			if (m_Pitch < -89.0f) {
				m_Pitch = -89.0f;
			}

			UpdateVectors();
		}

		glm::mat4 Camera::GetViewMatrix()
		{
			return glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
		}

		void Camera::UpdateVectors()
		{
			float ryaw = glm::radians(m_Yaw);
			float rpitch = glm::radians(m_Pitch);

			m_Front.x = cos(rpitch) * cos(ryaw);
			m_Front.y = sin(rpitch);
			m_Front.z = cos(rpitch) * sin(ryaw);
			m_Front = glm::normalize(m_Front);

			m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
			m_Up = glm::normalize(glm::cross(m_Right, m_Front));
		}
	}
}
