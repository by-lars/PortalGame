#pragma once

#include <string>
#include <GLFW/glfw3.h>
#include "Core/Settings.h"
#include "Asset/Registry.h"
#include "Graphics/Renderer.h"

namespace Engine {

	class Core {
	public:
		Core(const std::string& name);
		virtual ~Core();

		void MainLoop();
		virtual bool Init() = 0;
		virtual void Update() = 0;
		virtual void OnWindowSizeChanged(int width, int height) = 0;

		int GetWidth();
		int GetHeight();

		Settings Settings;
		ResourceRegistry Registry;
		Renderer m_Renderer;

		GLFWwindow* GetWindow();
		bool IsRunning();
		static Core& Get();
	private:

		static void OnWindowSizeChangedInternal(GLFWwindow* window, int width, int height);
		static void OnGLFWError(int code, const char* msg);

		int m_WindowWidth;
		int m_WindowHeight;

		GLFWwindow* m_Window;
		bool m_ShouldQuit;
		static Core* s_Instance;
	};

	Core* CreateApplication();
}
