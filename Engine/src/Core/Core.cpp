#include "Core/Base.h"
#include "Core/Core.h"
#include "Asset/Registry.h"

#include <GLFW/glfw3.h>


namespace Engine {
	Core* Core::s_Instance = nullptr;

	Core::Core(const std::string& name) : m_WindowWidth(800), m_WindowHeight(480), m_ShouldQuit(false) {
		s_Instance = this;

		Settings.App.Name = name;

		glfwInit();
		glfwSetErrorCallback(Core::OnGLFWError);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, name.c_str(), NULL, NULL);

		m_Renderer.Initialize();
	}

	GLFWwindow* Core::GetWindow() {
		return m_Window;
	}

	int Core::GetHeight() {
		return m_WindowHeight;
	}

	int Core::GetWidth() {
		return m_WindowWidth;
	}

	bool Core::IsRunning() {
		return !m_ShouldQuit;
	}

	void Core::MainLoop() {
		while (m_ShouldQuit == false) {
			m_ShouldQuit = glfwWindowShouldClose(m_Window);

			Update();
			m_Renderer.DrawFrame();

			glfwPollEvents();
		}

		m_Renderer.WaitForGPUIdle();
	}

	Core::~Core() {
		glfwTerminate();
	}

	Core& Core::Get() {
		return *s_Instance;
	}

	void Core::OnWindowSizeChangedInternal(GLFWwindow* window, int width, int height) {
		s_Instance->m_WindowHeight = height;
		s_Instance->m_WindowWidth = width;
		s_Instance->OnWindowSizeChanged(width, height);
	}

	void Core::OnGLFWError(int code, const char* msg) {
		ENGINE_ERROR("GLFW Error [" << code << "]: " << msg << std::endl);
	}
}