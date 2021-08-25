#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Base.h"
#include "Core/Application.h"

namespace Engine {
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name) {
		s_Instance = this;
		m_ShouldQuit = false;

		m_WindowWidth = 800;
		m_WindowHeight = 480;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif 

		m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, name.c_str(), NULL, NULL);
		pgAssert(m_Window != NULL, "Could not initialize window");

		glfwMakeContextCurrent(m_Window);

		int didGladLoad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		pgAssert(didGladLoad != 0, "Could not load OpenGL extensions");

		pgInfo("OpenGL Version: " << glGetString(GL_VERSION));

		glViewport(0, 0, 800, 480);

#ifdef _DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(Application::OnDebugMessage, nullptr);
		glfwSetFramebufferSizeCallback(m_Window, Application::OnWindowSizeChangedInternal);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
	}

	GLFWwindow* Application::GetWindow() {
		return m_Window;
	}

	int Application::GetHeight() {
		return m_WindowHeight;
	}

	int Application::GetWidth() {
		return m_WindowWidth;
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

	void Application::OnWindowSizeChangedInternal(GLFWwindow* window, int width, int height) {
		s_Instance->m_WindowHeight = height;
		s_Instance->m_WindowWidth = width;
		s_Instance->OnWindowSizeChanged(width, height);
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