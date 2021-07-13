#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Core/EntryPoint.h"

#include "OpenGL/Shader.h"
//#include "OpenGL/SimpleBuffer.h"
#include "OpenGL/Buffer.h"
#include "Asset/Cache.h"

#include "ECS/Scene.h"
#include "ECS/Systems/DummySystem.h"
#include "ECS/Components/Basic.h"

#include "Renderer/Renderer3D.h"
#include "Renderer/Camera.h"

using namespace Engine;

class Game : public Application {
public:
	float vertices[18] = {
		// positions         
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	GL::Buffer buffer;
	GL::Buffer objBuffer;
	std::shared_ptr<GL::Shader> shader;
	ECS::Scene scene;
	Asset::Cache cache;

	Renderer::Renderer3D renderer;
	Renderer::Camera camera;

	int m_VertCount;

	Game(const std::string &name) 
		: Application(name), cache(true), camera(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 0, 0, 0.1f) {

	}

	virtual bool Init() override {
		pgInfo("Init");

		glfwSetInputMode(Application::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		shader = cache.Get<GL::Shader>("assets/shader/testshader.glsl");
		shader->Use();

		std::vector<Renderer::Vertex> verts;
		Asset::LoadOBJ("assets/model/monke.obj", verts);

		glm::mat4 proj = glm::perspective(glm::radians(90.0f), 800.0f / 480.0f, 0.1f, 100.0f);
		glm::mat4 view = glm::mat4(1);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
		view = glm::rotate(view, (float)glfwGetTime()*100.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		shader->SetMatrix("proj", proj);
		shader->SetMatrix("view", view);

		Renderer::Mesh mesh{
			verts
		};

		std::vector<Renderer::Vertex> triangle_verts = {
			{ {0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
			{ {-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
			{ {0.0f,  0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}
		};

		Renderer::Mesh triangle{
			triangle_verts
		};

		renderer.Init();
		renderer.SubmitMesh(mesh);
		renderer.SubmitMesh(triangle);

		glm::mat4 transform(1.0f);
		
		for (int x = 0; x < 100; x++) {
			for (int y = 0; y < 100; y++) {
				for (int z = 0; z < 10; z++) {

					glm::mat4 trans(1.0f);
					trans = glm::translate(trans, glm::vec3(x * 5, y * 5, z * 5));
					renderer.AddInstance(&mesh, trans);

				}
			}
		}
		
		transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(0, -2, 0));
		renderer.AddInstance(&triangle, transform);

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

		//glDisable(GL_CULL_FACE);

		return PG_SUCCESS;
	}

	void ProcessInput(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.MoveDirection(Renderer::Camera::Direction::FOREWARD);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.MoveDirection(Renderer::Camera::Direction::LEFT);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.MoveDirection(Renderer::Camera::Direction::BACKWARD);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.MoveDirection(Renderer::Camera::Direction::RIGHT);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera.MoveDirection(Renderer::Camera::Direction::UP);
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			camera.MoveDirection(Renderer::Camera::Direction::DOWN);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);
		camera.ProcessMouse(x, y);
	}

	virtual void Update() override {
		cache.Update();
		scene.Update();

		ProcessInput(Application::GetWindow());

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->Use();

		glm::mat4 proj = glm::perspective(glm::radians(90.0f), 800.0f / 480.0f, 0.1f, 100.0f);

		shader->SetMatrix("view", camera.GetViewMatrix());
		shader->SetMatrix("proj", proj);


		/*buffer.Bind();
		buffer.Upload(vertices, 0, sizeof(vertices));
		glDrawArrays(GL_TRIANGLES, 0, 3);*/

		//objBuffer.Bind();
		//renderer.m_MeshBuffer.Bind();
		//glDrawArrays(GL_TRIANGLES, 0, m_VertCount);

		renderer.Render();


		Application::Update();
	}
};

Engine::Application* Engine::CreateApplication() {
	return new Game("SandBox");
}
