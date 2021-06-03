
#include <fstream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/EntryPoint.h"

#include "OpenGL/Shader.h"
#include "OpenGL/SimpleBuffer.h"

#include "Asset/Watcher.h"
#include "Asset/Cache.h"


#include "ImGui/ImGuiIncl.h"

using namespace PGame;
namespace fs = std::filesystem;

class Game : public Application {
public:
	Asset::Cache cache;

	Game(const std::string& name) : Application(name) {

	}

	~Game() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	virtual bool Init() override {
		pgInfo("Init");

		//std::shared_ptr<GL::Shader> shader = cache.Get<GL::Shader>("assets/shader/testshader.glsl");
		//pgInfo("Cached Shader: " << shader->id);
		//shader->Use();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui_ImplGlfw_InitForOpenGL(Application::GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");

		ImGui::StyleColorsDark();

		return PG_SUCCESS;
	}

	std::vector<std::shared_ptr<GL::Shader>> shaders;

	virtual void Update() override {
		cache.HotReloadChangedAssets();
		cache.CleanupCache();

		glClearColor(0.21f, 0.21f, 0.21f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Demo window");
		if (ImGui::Button("Add Shader!")) {
			
			shaders.push_back(cache.Get<GL::Shader>("assets/shader/testshader.glsl"));
			
		}

		if (ImGui::Button("Remove Shader!")) {
			if (shaders.size() > 0) {
				shaders.pop_back();
			}
		}

		ImGui::Text(std::to_string(shaders.size()).c_str());

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		Application::Update();
	}
};

PGame::Application* PGame::CreateApplication() {
	return new Game("Shade");
}
