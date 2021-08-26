
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

		r.SetClearColor({ 0.51f, 0.51f, 0.51f });

		return PG_SUCCESS;
	}

	virtual void Update() override {
		Asset::GetCache().Update();

		double time = glfwGetTime();

		r.Clear();
		for (int i = 0; i < 2000; i++) {
			r.DrawRect({ cos(time + i*5) * 100.0f + 200.0f, sin(time + i*5) * 100.0f + 200.0f }, { 64.0f, 64.0f }, {cos(time+i*500) * 0.5f + 0.2f, sin(time+i*100) * 0.5f + 0.5f, sin(time+i*100) * 0.5f + 0.5f });
		}
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
