#include "Renderer3D.h"

namespace PGame {
	namespace Renderer {
		const uint32_t Renderer3D::MAX_BUFFER_SIZE = 512 * 1024 * 1024;

		bool Renderer3D::Init() {
			glGenVertexArrays(1, &m_RenderVAO);

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
				MAX_BUFFER_SIZE,
				m_RenderVAO);

			m_InstanceDataBuffer.Init(
				GL::BufferTypes::ARRAY_BUFFER,
				GL::BufferUsages::DYNAMIC_DRAW,
				MAX_BUFFER_SIZE,
				m_RenderVAO);
			m_InstanceDataBuffer.SetFormat(
				3, 
				{GL::BufferElement(GL::DataTypes::MAT4f, 1, 1)});


			return true;
		}

		void Renderer3D::SubmitMesh(const Mesh& mesh) {
			m_MeshBuffer.PushBack((void*)mesh.Vertecies.data(), mesh.GetSizeInBytes());
		}

		void Renderer3D::AddInstance(Mesh* mesh, const glm::mat4& transform) {
			m_InstanceDataBuffer.PushBack((void*)&transform, sizeof(transform));
			
			if (m_MeshToCommandIndex.find(mesh) == m_MeshToCommandIndex.end()) {
				DrawArraysIndirectCommand cmd;
				cmd.InstanceCount = 1;
				cmd.VertexCount = mesh->Vertecies.size();
				cmd.BaseInstance = m_CurrentBaseInstance;
				cmd.FirstIndex = m_CurrentVertexIndex;

				m_CurrentBaseInstance += 1;
				m_CurrentVertexIndex += mesh->Vertecies.size();
			
				m_CommandBuffer.PushBack((void*)&cmd, sizeof(cmd));
				m_RenderCommands.push_back(cmd);
				m_MeshToCommandIndex[mesh] = m_RenderCommands.size() - 1;
			}
			else {
				size_t index = m_MeshToCommandIndex[mesh];
				auto& cmd = m_RenderCommands[index];
				cmd.InstanceCount += 1;
				m_CurrentBaseInstance += 1;
				m_CommandBuffer.Upload((void*)&m_RenderCommands[index], m_MeshToCommandIndex[mesh] * sizeof(cmd), sizeof(cmd));
			}
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