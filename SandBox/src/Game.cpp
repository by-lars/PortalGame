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

	Renderer::R3D renderer;
	Renderer::Camera camera;

	int m_VertCount;

	Game(const std::string &name) 
		: Application(name), camera(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 0, 0, 0.1f) {

	}

	virtual void OnWindowSizeChanged(int width, int height) override {
		renderer.SetResolution(width, height);
	}

	virtual bool Init() override {
		pgInfo("Init");

		glfwSetInputMode(Application::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		
		std::shared_ptr<Renderer::Mesh> mesh
		= Asset::Get<Renderer::Mesh>("assets/model/monke.obj");



		renderer.Init(Application::GetWidth(), Application::GetHeight(), 90.0f);
		renderer.SubmitMesh(mesh);

		glm::mat4 transform(1.0f);
		
		for (int x = 0; x < 40; x++) {
			for (int y = 0; y < 10; y++) {
				for (int z = 0; z < 40; z++) {

					glm::mat4 trans(1.0f);
					trans = glm::translate(trans, glm::vec3(x *4, y *4, z *4));

					trans = glm::rotate(trans, glm::radians(glm::linearRand(0.0f,360.0f)), glm::vec3(1,1,1));
					renderer.AddInstance(mesh, trans);

				}
			}
		}
		

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
		Asset::GetCache().Update();
		scene.Update();

		ProcessInput(Application::GetWindow());

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		renderer.SetViewMatrix(camera.GetViewMatrix());
		renderer.Render();


		Application::Update();
	}
};

Engine::Application* Engine::CreateApplication() {
	return new Game("SandBox");
}
