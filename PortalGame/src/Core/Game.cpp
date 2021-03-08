#include <cmath>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Base.h"
#include "Core/Application.h"

#include "OpenGL/Shader.h"
#include "OpenGL/SimpleBuffer.h"
#include "ECS/ComponentArray.h"
#include "ECS/ComponentPool.h"

namespace PGame {
	class Game : public Application {
	public:
		float vertices[18] = {
			// positions         
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f , 0.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
		};

		Ref<GL::SimpleBuffer> buffer;
		Ref<GL::Shader> shader;

		Game() {
			buffer = CreateRef<GL::SimpleBuffer>(sizeof(vertices));
			shader = CreateRef<GL::Shader>();
		}

		virtual bool Init() override {
			pgInfo("Init");
			std::ifstream file("assets/shader/testshader.glsl");

			if (!file) {
				pgError("Coudl not open file :(");
				return PG_FAILURE;
			}

			file.seekg(0, std::ios::end);
			size_t size = file.tellg();
			std::string source(size, ' ');

			file.seekg(0, std::ios::beg);
			file.read(&source[0], size);

			shader->Load(source);
			shader->Use();

			buffer->Upload(vertices, sizeof(vertices));
			buffer->SetFormat({ GL::BufferElement(GL_FLOAT, 3), GL::BufferElement(GL_FLOAT, 3) });
			buffer->Bind();

			ECS::ComponentArray<int> mySet;
			mySet.Insert(0, 123);
			mySet.Insert(1, 423);
			mySet.Insert(2, 444);
			mySet.Erase(1);
			mySet.Insert(850, 4467);
			mySet.Insert(851, 1234);
			mySet.Erase(0);
			mySet.Insert(15, 69);
			mySet.Insert(16, 69);
			mySet.Insert(17, 69);
			mySet.Insert(18, 69);
			mySet.Insert(19, 69);
			mySet.Insert(20, 69);
			mySet.Insert(21, 69);

			ECS::ComponentPool pool;
			pool.RegisterComponent<int>();
			pool.RegisterComponent<float>();
			pool.RegisterComponent<float>();


			pgInfo(mySet.Get(0));

			return PG_SUCCESS;
		}

		virtual void Update() override {
			glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			vertices[0] = cos(glfwGetTime());
			vertices[4] = tan(glfwGetTime());
			vertices[7] = sin(glfwGetTime());
			buffer->Upload(vertices, sizeof(vertices));

			glDrawArrays(GL_TRIANGLES, 0, 3);

			Application::Update();
		}
	};

	Application* CreateApplication() {
		return new Game();
	}
}
