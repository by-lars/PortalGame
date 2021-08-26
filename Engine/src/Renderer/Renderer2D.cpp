#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Renderer2D.h"
#include "Asset/Cache.h"

namespace Engine {
	namespace Renderer {
		const int MAX_QUADS = 2000;

		float QuadVerticies[] = {
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		void R2D::Init(int width, int height) {
			pgDebug("Initializing Renderer2D");
			pgAssert(width > 0, "Render Width must be positive");
			pgAssert(height > 0, "Render Height must be positive");

			m_RenderWidth = width;
			m_RenderHeight = height;

			m_Shader = Asset::Get<GL::Shader>("assets/shader/shader2d.glsl");
			m_Shader->SetInt("uTex", 0);

			m_Texture = Asset::Get<GL::Texture>("assets/texture/albedo.png");

			m_MeshBuffer.Init(
				GL::BufferTypes::ARRAY_BUFFER, 
				GL::BufferUsages::DYNAMIC_DRAW, 
				sizeof(QuadVerticies));
			m_MeshBuffer.SetFormat({
				GL::BufferElement(GL::DataTypes::FLOAT, 2), // Position		X,Y
				GL::BufferElement(GL::DataTypes::FLOAT, 2)  // Texture Pos	U,V
			});
			m_MeshBuffer.PushBack(QuadVerticies, sizeof(QuadVerticies));

			m_TransformBuffer.Init(
				GL::BufferTypes::ARRAY_BUFFER, 
				GL::BufferUsages::DYNAMIC_DRAW, 
				sizeof(RectangleData) * MAX_QUADS, 
				m_MeshBuffer.GetVAO() //Use the same VAO as the MeshBuffer 
			);

			//Set Divisor to 1, to update attribute for every instance
			m_TransformBuffer.SetFormat(
				2, //Start the matrix after pos and tex from the meshbuffer 
				{GL::BufferElement(GL::DataTypes::FLOAT, 3, 1), // Color Vec3
				GL::BufferElement(GL::DataTypes::MAT4f, 1, 1)} //Transformm mat4 
			);


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
			m_TransformBuffer.Upload(m_Rectangles.data(), 0, sizeof(m_Rectangles[0]) * m_Rectangles.size());
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_Rectangles.size());
	
			//Clean up the old draw jobs
			m_Rectangles.clear();
		}

		void R2D::DrawRect(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color) {
			glm::mat4 transform = glm::mat4(1.0);
			//TODO(@Lars): Make sure this is in the right order
			transform = glm::translate(transform, glm::vec3(pos, 0));
			transform = glm::scale(transform, glm::vec3(size, 1.0f));
			
			m_Rectangles.push_back(RectangleData{ color, transform });
		}
	}
}