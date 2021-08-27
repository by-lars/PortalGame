#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Renderer2D.h"
#include "Asset/Cache.h"

namespace Engine {
	namespace Renderer {
		Quad QuadVertecies = {
			QuadVertex {{0.0f, 1.0f}, {0.0f, 1.0f}},
			QuadVertex {{1.0f, 0.0f}, {1.0f, 0.0f}},
			QuadVertex {{0.0f, 0.0f}, {0.0f, 0.0f}},

			QuadVertex {{0.0f, 1.0f}, {0.0f, 1.0f}},
			QuadVertex {{1.0f, 1.0f}, {1.0f, 1.0f}},
			QuadVertex {{1.0f, 0.0f}, {1.0f, 0.0f}}
		};

		void R2D::Init(int width, int height) {
			pgDebug("Initializing Renderer2D");
			pgAssert(width > 0, "Render Width must be positive");
			pgAssert(height > 0, "Render Height must be positive");

			m_RenderWidth = width;
			m_RenderHeight = height;

			m_Shader = Asset::Get<GL::Shader>("assets/shader/shader2d.glsl");
			m_Shader->SetInt("uTex", 0);

			m_Texture = Asset::Get<GL::Texture>("assets/texture/albedo.jpg");

			m_QuadBuffer.Init(
				GL::BufferTypes::ARRAY_BUFFER, 
				GL::BufferUsages::DYNAMIC_DRAW, 
				MAX_QUADS * sizeof(Quad));

			//Set Divisor to 1 so everything updates for each instance
			m_QuadBuffer.SetFormat({
				GL::BufferElement(GL::DataTypes::FLOAT, 2, 1), // Position		X,Y
				GL::BufferElement(GL::DataTypes::FLOAT, 2, 1)  // Texture Pos	U,V
			});

			m_RectanglesToDraw = 0;
					
			UpdateProjectionMatrix();
		}

		void R2D::SetResolution(int width, int height) {
			if (width > 0 && height > 0) {
				m_RenderWidth = width;
				m_RenderHeight = height;
				glViewport(0, 0, m_RenderWidth, m_RenderHeight);
				UpdateProjectionMatrix();
			}
		}

		void R2D::UpdateProjectionMatrix() {
			glm::mat4 projection = glm::ortho(0.0f, (float)m_RenderWidth, (float)m_RenderHeight, 0.0f, -1.0f, 1.0f);
			m_Shader->SetMatrix("uProj", projection);
		}

		void R2D::SetClearColor(const glm::vec3& color) {
			glClearColor(color.r, color.g, color.b, 1.0f);
		}

		void R2D::Clear() {
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void R2D::Finish() {
			//Draw Rectangles
			m_QuadBuffer.Upload(m_Quads.data(), 0, sizeof(Quad) * m_RectanglesToDraw);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_RectanglesToDraw);
			glDrawArrays(GL_TRIANGLES, 0, 6 * m_RectanglesToDraw);
			m_RectanglesToDraw = 0;
		}

		void R2D::DrawRect(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color) {
			glm::mat4 transform = glm::mat4(1.0);

			transform = glm::translate(transform, glm::vec3(pos, 0));
			transform = glm::scale(transform, glm::vec3(size, 1.0f));

			Quad quad = QuadVertecies;

			for (int i = 0; i < 6; i++) {
				glm::vec4 pos(quad.Vertecies[i].Pos, 0.0, 0.0);
				pos = transform * pos;
				quad.Vertecies[i].Pos = pos;
			}

			m_Quads[m_RectanglesToDraw] = quad;

			m_RectanglesToDraw++;
		}
	}
}