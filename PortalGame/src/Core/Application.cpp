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
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif 

		m_Window = glfwCreateWindow(800, 480, name.c_str(), NULL, NULL);
		pgAssert(m_Window != NULL, "Could not initialize window");

		glfwMakeContextCurrent(m_Window);

		int didGladLoad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		pgAssert(didGladLoad != 0, "Could not load OpenGL extensions");

		pgInfo("OpenGL Version: " << glGetString(GL_VERSION));

		glViewport(0, 0, 800, 480);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

#ifdef _DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(Application::OnDebugMessage, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
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

	void Application::OnDebugMessage(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {
		//if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:		 pgError("GL (" << id << "): " << message); break;
			case GL_DEBUG_SEVERITY_MEDIUM:		 pgError("GL (" << id << "): " << message);  break;
			case GL_DEBUG_SEVERITY_LOW:			 pgWarn("GL (" << id << "): " << message);  break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: pgInfo("GL (" << id << "): " << message); break;
		}
	}
}