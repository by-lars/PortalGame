#include "Renderer3D.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Asset/Cache.h"

namespace Engine {
	namespace Renderer {
		const uint32_t Renderer3D::MAX_BUFFER_SIZE = 128 * 1024 * 1024;

		void Renderer3D::Init(int width, int height, float fov) {
			glGenVertexArrays(1, &m_RenderVAO);

			m_Shader = Asset::Cache::Instance().Get<GL::Shader>("assets/shader/testshader.glsl");

			m_MeshBuffer.Init(
				GL::BufferTypes::ARRAY_BUFFER,
				GL::BufferUsages::STATIC_DRAW,
				MAX_BUFFER_SIZE, 
				m_RenderVAO);
			m_MeshBuffer.SetFormat({
				GL::BufferElement(GL::DataTypes::FLOAT, 3),
				GL::BufferElement(GL::DataTypes::FLOAT, 2),
				GL::BufferElement(GL::DataTypes::FLOAT, 3)});

			m_CommandBuffer.Init(
				GL::BufferTypes::DRAW_INDIRECT_BUFFER,
				GL::BufferUsages::DYNAMIC_DRAW,
				MAX_BUFFER_SIZE / 4,
				m_RenderVAO);

			m_InstanceDataBuffer.Init(
				GL::BufferTypes::ARRAY_BUFFER,
				GL::BufferUsages::DYNAMIC_DRAW,
				MAX_BUFFER_SIZE / 4,
				m_RenderVAO);
			m_InstanceDataBuffer.SetFormat(
				3, {GL::BufferElement(GL::DataTypes::MAT4f, 1, 1)});

			m_Fov = fov;
			m_RenderWidth = width;
			m_RenderHeight = height;
			m_ClippingZNear = 0.1f;
			m_ClippingZFar = 100.0f;
			UpdateProjectionMatrix();
			SetViewMatrix(glm::mat4(1));
		}

		Renderer3D::~Renderer3D() {
			glDeleteVertexArrays(1, &m_RenderVAO);
		}

		void Renderer3D::SetViewMatrix(const glm::mat4 view) {
			m_ViewMatrix = view;
			m_Shader->SetMatrix("uView", m_ViewMatrix);
		}

		void Renderer3D::UpdateProjectionMatrix() {
			m_ProjectionMatrix = glm::perspective(m_Fov, (float)m_RenderWidth / (float)m_RenderHeight, m_ClippingZNear, m_ClippingZFar);
			m_Shader->SetMatrix("uProj", m_ProjectionMatrix);
		}

		void Renderer3D::SetFov(float fov) {
			m_Fov = fov;
			UpdateProjectionMatrix();
		}

		void Renderer3D::SetResolution(int width, int height) {
			if (width > 0 && height > 0) {
				m_RenderWidth = width;
				m_RenderHeight = height;
				glViewport(0, 0, m_RenderWidth, m_RenderHeight);
				UpdateProjectionMatrix();
			}
		}

		void Renderer3D::SetClippingDistance(float zNear, float zFar) {
			m_ClippingZNear = zNear;
			m_ClippingZFar = zFar;
			UpdateProjectionMatrix();
		}


		void Renderer3D::SubmitMesh(Mesh* mesh) {
			MeshInfo info;
			info.VertexBufferIndex = m_MeshBuffer.GetCurrentOffsetIndex();
			m_MeshInfo[mesh] = info;

			m_MeshBuffer.PushBack((void*)mesh->Vertecies.data(), mesh->GetSizeInBytes());
		}

		void Renderer3D::AddInstance(Mesh* mesh, const glm::mat4& transform) {
			
			MeshInfo& info = m_MeshInfo[mesh];

			if (m_LastMesh == mesh) {	
				auto& info = m_MeshInfo[mesh];
				auto& command = m_RenderCommands.back();
				command.InstanceCount += 1;
				m_CommandBuffer.Upload((void*)&command, info.CommandBufferOffset, sizeof(command));
			}
			else {
				info.CommandBufferOffset = m_CommandBuffer.GetCurrentOffsetBytes();
				info.InstanceBufferIndex = m_InstanceDataBuffer.GetCurrentOffsetIndex();

				DrawArraysIndirectCommand command;
				command.InstanceCount = 1;
				command.VertexCount = mesh->Vertecies.size();
				command.BaseInstance = info.InstanceBufferIndex;
				command.FirstIndex = info.VertexBufferIndex;

				m_CommandBuffer.PushBack((void*)&command, sizeof(command));
				m_RenderCommands.push_back(command);
			}

			m_InstanceDataBuffer.PushBack((void*)&transform, sizeof(transform));

			m_LastMesh = mesh;
		}

		void Renderer3D::Render() {
			glBindVertexArray(m_RenderVAO);
			glMultiDrawArraysIndirect(
				GL_TRIANGLES,
				0,
				m_RenderCommands.size(),
				0);
		}
	}
}