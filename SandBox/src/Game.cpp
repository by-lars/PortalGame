#include <fstream>
#include <memory>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/random.hpp>

#include "Core/EntryPoint.h"


using namespace Engine;

class Game : public Engine::Core {
public:
	Game() : Core("SandBox") {

	}

	virtual void OnWindowSizeChanged(int width, int height) override {
	
	}

	virtual bool Init() override {
		ENGINE_INFO("Init");
		//glfwSetInputMode(Application::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);


		return ENGINE_SUCCESS;
	}

	void ProcessInput(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			int inputMode = glfwGetInputMode(window, GLFW_CURSOR);
			if (inputMode == GLFW_CURSOR_NORMAL) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}
	}

	float lastFrameTime;
	float frameDelta;

	virtual void Update() override {
		float thisFrameTime = glfwGetTime();
		frameDelta = thisFrameTime - lastFrameTime;
		lastFrameTime = thisFrameTime;
	
		std::stringstream ss;
		ss << "FPS: " << (1 / frameDelta);

		glfwSetWindowTitle(GetWindow(), ss.str().c_str());

		ProcessInput(Core::GetWindow());
	}
};

Engine::Core* Engine::CreateApplication() {
	return new Game();
}
