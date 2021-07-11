#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Core/EntryPoint.h"

#include "OpenGL/Shader.h"
#include "OpenGL/SimpleBuffer.h"
#include "Asset/Cache.h"

#include "ECS/Scene.h"
#include "ECS/Systems/DummySystem.h"
#include "ECS/Components/Basic.h"


using namespace PGame;

class Game : public Application {
public:
	float vertices[18] = {
		// positions         
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	std::shared_ptr<GL::SimpleBuffer> buffer;
	std::shared_ptr<GL::SimpleBuffer> objBuffer;
	std::shared_ptr<GL::Shader> shader;
	ECS::Scene scene;
	Asset::Cache cache;
	int m_VertCount;

	Game(const std::string &name) : Application(name), cache(true) {
		buffer = std::make_shared<GL::SimpleBuffer>(sizeof(vertices));
	}

	virtual bool Init() override {
		pgInfo("Init");

		shader = cache.Get<GL::Shader>("assets/shader/testshader.glsl");
		shader->Use();

		buffer->Upload(vertices, sizeof(vertices));
		buffer->SetFormat({ GL::BufferElement(GL_FLOAT, 3), GL::BufferElement(GL_FLOAT, 3) });
		buffer->Bind();

		std::vector<Renderer::Vertex> verts;
		Asset::LoadOBJ("assets/model/monke2.obj", verts);
		objBuffer = std::make_shared<GL::SimpleBuffer>(verts.size() *  sizeof(verts));
		objBuffer->Upload(verts.data(), verts.size() * sizeof(verts));
		objBuffer->SetFormat({ GL::BufferElement(GL_FLOAT, 3), GL::BufferElement(GL_FLOAT, 2),  GL::BufferElement(GL_FLOAT, 3) });
		objBuffer->Bind();
		m_VertCount = verts.size();
		pgInfo("m_VertCount: " << m_VertCount);

		glm::mat4 proj = glm::perspective(glm::radians(90.0f), 800.0f / 480.0f, 0.1f, 100.0f);
		glm::mat4 view = glm::mat4(1);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
		view = glm::rotate(view, (float)glfwGetTime()*100.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		shader->SetMatrix("proj", proj);
		shader->SetMatrix("view", view);

		//scene.RegisterComponent<ECS::Transform>();
		//scene.RegisterComponent<ECS::Tag>();
		//scene.RegisterSystem<ECS::DummySystem>();
		//
		//ECS::Entity ent1 = scene.CreateEntity();
		//auto& tag = scene.Assign<ECS::Tag>(ent1);
		//tag.Tag = "Hey!";

		//ECS::Entity ent2 = scene.CreateEntity();
		//auto& tag2 = scene.Assign<ECS::Tag>(ent2);
		//tag2.Tag = "YOOO!";

		return PG_SUCCESS;
	}

	virtual void Update() override {
		cache.Update();
		scene.Update();

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->Use();

		glm::mat4 proj = glm::perspective(glm::radians(90.0f), 800.0f / 480.0f, 0.1f, 100.0f);
		glm::mat4 view = glm::mat4(1);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
		view = glm::rotate(view, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		shader->SetMatrix("view", view);
		shader->SetMatrix("proj", proj);


		buffer->Bind();
		buffer->Upload(vertices, sizeof(vertices));
		glDrawArrays(GL_TRIANGLES, 0, 3);

		objBuffer->Bind();
		glDrawArrays(GL_TRIANGLES, 0, m_VertCount);

		Application::Update();
	}
};

PGame::Application* PGame::CreateApplication() {
	return new Game("SandBox");
}
