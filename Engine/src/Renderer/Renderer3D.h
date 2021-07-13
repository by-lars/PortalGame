#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>

#include "Renderer/Data.h"
#include "OpenGL/Buffer.h"


namespace Engine {
	namespace Renderer {
		class Renderer3D {
		public:
			Renderer3D() = default;

			bool Init();
			void SubmitMesh(const Mesh& mesh);
			void AddInstance(Mesh* mesh, const glm::mat4& transform);

			void Render();

		private:
			static const uint32_t MAX_BUFFER_SIZE;

			GLuint m_RenderVAO;

			size_t m_CurrentBaseInstance;
			size_t m_CurrentVertexIndex;

			GL::Buffer m_MeshBuffer;
			GL::Buffer m_CommandBuffer;
			GL::Buffer m_InstanceDataBuffer;

			std::vector<DrawArraysIndirectCommand> m_RenderCommands;
			std::unordered_map<Mesh*, size_t> m_MeshToCommandIndex;
		};
	}
}