#pragma once

#include <string>
#include <GLFW/glfw3.h>

typedef struct GLFWwindow GLFWwindow;

namespace Engine {
	class Application {
	public:
		Application(const std::string& name, Application* derived);
		virtual ~Application();

		virtual bool Init() = 0;
		virtual void Update() = 0;
		virtual void OnWindowSizeChanged(int width, int height) = 0;

		GLFWwindow* GetWindow();
		bool IsRunning();
		static Application& Get();
		static void APIENTRY OnDebugMessage(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
	private:
		static void OnWindowSizeChangedInternal(GLFWwindow* window, int width, int height);

		GLFWwindow* m_Window;
		bool m_ShouldQuit;
		static Application* s_Instance;
		static Application* s_DerivedClass;
	};

	Application* CreateApplication();
}
