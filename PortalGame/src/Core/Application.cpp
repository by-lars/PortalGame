#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Base.h"
#include "Core/Application.h"

namespace PGame {
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name) {
		s_Instance = this;
		m_ShouldQuit = false;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(800, 480, name.c_str(), NULL, NULL);
		if (m_Window == NULL) {
			pgError("Could not initialize window");
			glfwTerminate();
			return; //TODO ADD ASSERT
		}

		glfwMakeContextCurrent(m_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			pgError("Could not load OpenGL extensions");
			glfwTerminate();
			return; //ADD ASSERT!
		}

		glViewport(0, 0, 800, 480);
	}

	GLFWwindow* Application::GetWindow() {
		return m_Window;
	}

	bool Application::IsRunning() {
		return !m_ShouldQuit;
	}

	void Application::Update() {
		m_ShouldQuit = glfwWindowShouldClose(m_Window);
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	Application::~Application() {
		glfwTerminate();
	}

	Application& Application::Get() {
		return *s_Instance;
	}
}