#ifndef PG_CORE_APPLICATION_H
#define PG_CORE_APPLICATION_H

#include <string>

typedef struct GLFWwindow GLFWwindow;

namespace PGame {
	class Application {
	public:
		Application(const std::string& name, size_t width, size_t height);
		virtual ~Application();

		virtual bool Init() = 0;
		virtual void Update() = 0;

		bool IsRunning;

		static Application& Get();
	private:
		GLFWwindow* m_Window;
		static Application* s_Instance;
	};

	Application* CreateApplication();
}

#endif