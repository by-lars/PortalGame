#pragma once

#include <string>

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
	private:
		bool m_ShouldQuit;
		GLFWwindow* m_Window;
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
