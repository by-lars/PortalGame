
#include <fstream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/EntryPoint.h"

#include "OpenGL/Texture.h"

#include "Renderer/Renderer2D.h"

#include "Asset/Cache.h"

using namespace Engine;
namespace fs = std::filesystem;

class Game : public Application {
public:

	Renderer::R2D r;

	Game(const std::string& name) : Application(name) {
		r.Init(Application::GetWidth(), Application::GetHeight());
	}

	~Game() {

	}

	virtual bool Init() override {
		pgInfo("Init");

		r.SetClearColor({ 0.21f, 0.21f, 0.21f });

		return PG_SUCCESS;
	}

	virtual void Update() override {
		Asset::GetCache().Update();

		double time = glfwGetTime();

		r.Clear();
		r.DrawRect({ 0.0, 0.0 }, { Application::GetWidth(), Application::GetHeight() }, glm::vec3(1.0f, 1.0f, 1.0f));
		r.DrawRect({ 500.0, 500.0 }, { Application::GetWidth(), Application::GetHeight() }, glm::vec3(1.0f, 1.0f, 1.0f));
		r.Finish();

		Application::Update();
	}

	virtual void OnWindowSizeChanged(int width, int height) {
		r.SetResolution(width, height);
	}
};

Engine::Application* Engine::CreateApplication() {
	return new Game("Shade");
}
