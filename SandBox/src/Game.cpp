#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/random.hpp>

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

	ECS::Scene scene;

	Renderer::Renderer3D renderer;
	Renderer::Camera camera;

	int m_VertCount;

	Game(const std::string &name) 
		: Application(name, this), camera(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 0, 0, 0.1f) {

	}

	virtual void OnWindowSizeChanged(int width, int height) override {
		renderer.SetResolution(width, height);
	}

	virtual bool Init() override {
		pgInfo("Init");

		glfwSetInputMode(Application::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		
		std::vector<Renderer::Vertex> verts;
		Asset::LoadOBJ("assets/model/monke.obj", verts);


		Renderer::Mesh mesh{
			verts
		};

		std::vector<Renderer::Vertex> triangle_verts = {
			{ {0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
			{ {-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
			{ {0.0f,  0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}
		};

		std::vector<Renderer::Vertex> triangle_verts2 = {
			{ {0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
			{ {-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
			{ {0.0f,  0.5f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}
		};

		Renderer::Mesh triangle{
			triangle_verts
		};

		Renderer::Mesh triangle2{
			triangle_verts2
		};


		renderer.Init(800, 400, 90.0f);
		renderer.SubmitMesh(&mesh);
		renderer.SubmitMesh(&triangle);

		glm::mat4 transform(1.0f);
		
		for (int x = 0; x < 100; x++) {
			for (int y = 0; y < 10; y++) {
				for (int z = 0; z < 20; z++) {

					glm::mat4 trans(1.0f);
					trans = glm::translate(trans, glm::vec3(x * 5, y * 5, z * 5));

					trans = glm::rotate(trans, glm::radians(glm::linearRand(0.0f,360.0f)), glm::vec3(1,1,1));
					renderer.AddInstance(&mesh, trans);

				}
			}
		}
		

		for (int x = 0; x < 100; x++) {
			for (int y = 0; y < 10; y++) {
				for (int z = 0; z < 100; z++) {
					glm::mat4 trans(1.0f);
					trans = glm::translate(trans, glm::vec3(x , y + glm::perlin(glm::vec3(x / 10.0f, y / 10.0f, z / 10.0f)) * 10.0f, z));
					trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(1, 0, 0));
					renderer.AddInstance(&triangle, trans);
				}
			}
		}

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

		if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			int inputMode = glfwGetInputMode(window, GLFW_CURSOR);
			if (inputMode == GLFW_CURSOR_NORMAL) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}

	
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);
		camera.ProcessMouse(x, y);
	}

	virtual void Update() override {
		Asset::Cache::Instance().Update();
		scene.Update();

		ProcessInput(Application::GetWindow());

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		renderer.SetViewMatrix(camera.GetViewMatrix());
		renderer.Render();


		Application::Update();
	}
};

Engine::Application* Engine::CreateApplication() {
	return new Game("SandBox");
}
