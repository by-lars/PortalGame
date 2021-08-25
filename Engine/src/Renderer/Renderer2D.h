#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "OpenGL/Shader.h"
#include "OpenGL/Buffer.h"

namespace Engine {
	namespace Renderer {
#pragma pack(push)
		struct RectangleData {
			glm::vec3 Color;
			glm::mat4 Transform;
		};
#pragma pack(pop)

		class R2D {
		public:
			R2D() = default;

			void Init(int width, int height);
			void SetResolution(int width, int height);

			void Finish();
			void SetClearColor(const glm::vec3& color);
			void Clear();

			void DrawRect(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color);
		private:
			void UpdateProjectionMatrix();

			int m_RenderWidth;
			int m_RenderHeight;

			std::shared_ptr<GL::Shader> m_Shader;
			GL::Buffer m_MeshBuffer;
			GL::Buffer m_TransformBuffer;

			std::vector<RectangleData> m_Rectangles;
		};
	}
}