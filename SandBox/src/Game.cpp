#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/EntryPoint.h"

#include "OpenGL/Shader.h"
#include "OpenGL/SimpleBuffer.h"

#include "ECS/Scene.h"

#include <fstream>

using namespace PGame;

class Game : public Application {
public:
	float vertices[18] = {
		// positions         
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	Ref<GL::SimpleBuffer> buffer;
	Ref<GL::Shader> shader;
	ECS::Scene scene;

	Game(const std::string &name) : Application(name) {
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

		return PG_SUCCESS;
	}

	virtual void Update() override {
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//vertices[0] = cos(glfwGetTime());
		//vertices[4] = tan(glfwGetTime());
		//vertices[7] = sin(glfwGetTime());
		buffer->Upload(vertices, sizeof(vertices));

		glDrawArrays(GL_TRIANGLES, 0, 3);

		Application::Update();
	}
};

PGame::Application* PGame::CreateApplication() {
	return new Game("SandBox");
}
