
#include <fstream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/EntryPoint.h"

#include "OpenGL/Shader.h"
#include "OpenGL/SimpleBuffer.h"
#include "OpenGL/RenderTexture.h"

#include "Asset/Watcher.h"
#include "Asset/Cache.h"


#include "ImGui/ImGuiIncl.h"

using namespace Engine;
namespace fs = std::filesystem;

class Game : public Application {
public:
	Asset::Cache cache;
	std::shared_ptr<GL::Shader> shader;
	std::unique_ptr<GL::SimpleBuffer> rectBuffer;
	std::unique_ptr<GL::RenderTexture> renderTexture;

	Game(const std::string& name) : Application(name), cache(true) {

	}

	~Game() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	virtual bool Init() override {
		pgInfo("Init");

		//Setup IMGUI
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplGlfw_InitForOpenGL(Application::GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");
		ImGui::StyleColorsDark();

		//Load Shader
		shader = cache.Get<GL::Shader>("assets/shader/testshader.glsl");
		
		//Load rectangle vertecies
		float vertices[36] = {
			0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // top right
			0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f, // bottom right
		   -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // top left 

			0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f, // bottom right
		   -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom left
		   -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f // top left
		};
		
		rectBuffer = std::make_unique<GL::SimpleBuffer>(sizeof(vertices));
		rectBuffer->SetFormat({ GL::BufferElement(GL_FLOAT, 3), GL::BufferElement(GL_FLOAT, 3) });
		rectBuffer->Upload(vertices, sizeof(vertices));
		rectBuffer->Bind();

		renderTexture = std::make_unique<GL::RenderTexture>(800, 400);

		return PG_SUCCESS;
	}

	virtual void Update() override {
		cache.Update();

		glClearColor(0.21f, 0.21f, 0.21f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		renderTexture->Bind();
			shader->Use();
			rectBuffer->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
		renderTexture->UnBind();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Demo window");
		//ImGui::GetWindowDrawList()->AddImage(renderTexture->GetTextureId(), )
		ImGui::Image((void*)renderTexture->GetTextureId(), ImGui::GetWindowSize(), ImVec2(0,1), ImVec2(1,0));
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		Application::Update();
	}
};

Engine::Application* Engine::CreateApplication() {
	return new Game("Shade");
}
