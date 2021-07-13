#pragma once

#include <string>
#include <GLFW/glfw3.h>

typedef struct GLFWwindow GLFWwindow;

namespace PGame {
	class Application {
	public:
		Application(const std::string& name);
		virtual ~Application();

		virtual bool Init() = 0;
		virtual void Update() = 0;

		GLFWwindow* GetWindow();
		bool IsRunning();
		static Application& Get();
		static void APIENTRY OnDebugMessage(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
	private:
		GLFWwindow* m_Window;
		bool m_ShouldQuit;
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
