#pragma once

#include <string>
#include <GLFW/glfw3.h>

namespace Engine {
	class Application {
	public:
		Application(const std::string& name);
		virtual ~Application();

		virtual bool Init() = 0;
		virtual void Update() = 0;
		virtual void OnWindowSizeChanged(int width, int height) = 0;

		int GetWidth();
		int GetHeight();

		GLFWwindow* GetWindow();
		bool IsRunning();
		static Application& Get();
	private:
		static void APIENTRY OnDebugMessage(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
		static void OnWindowSizeChangedInternal(GLFWwindow* window, int width, int height);

		int m_WindowWidth;
		int m_WindowHeight;

		GLFWwindow* m_Window;
		bool m_ShouldQuit;
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
