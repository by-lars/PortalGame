#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <memory>


#include "Core/EntryPoint.h"

#include "OpenGL/Shader.h"
#include "OpenGL/SimpleBuffer.h"
#include "Asset/Cache.h"

#include "ECS/Scene.h"
#include "ECS/Systems/DummySystem.h"
#include "ECS/Components/Basic.h"


using namespace PGame;

class Game : public Application {
public:
	float vertices[18] = {
		// positions         
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	std::shared_ptr<GL::SimpleBuffer> buffer;
	std::shared_ptr<GL::Shader> shader;
	ECS::Scene scene;
	Asset::Cache cache;

	Game(const std::string &name) : Application(name), cache(true) {
		buffer = std::make_shared<GL::SimpleBuffer>(sizeof(vertices));
	}

	virtual bool Init() override {
		pgInfo("Init");

		shader = cache.Get<GL::Shader>("assets/shader/testshader.glsl");
		shader->Use();

		buffer->Upload(vertices, sizeof(vertices));
		buffer->SetFormat({ GL::BufferElement(GL_FLOAT, 3), GL::BufferElement(GL_FLOAT, 3) });
		buffer->Bind();
			
		scene.RegisterComponent<ECS::Transform>();
		scene.RegisterComponent<ECS::Tag>();
		scene.RegisterSystem<ECS::DummySystem>();
		
		ECS::Entity ent1 = scene.CreateEntity();
		auto& tag = scene.Assign<ECS::Tag>(ent1);
		tag.Tag = "Hey!";

		ECS::Entity ent2 = scene.CreateEntity();
		auto& tag2 = scene.Assign<ECS::Tag>(ent2);
		tag2.Tag = "YOOO!";

		return PG_SUCCESS;
	}

	virtual void Update() override {
		cache.Update();
		scene.Update();

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->Use();
		buffer->Upload(vertices, sizeof(vertices));

		glDrawArrays(GL_TRIANGLES, 0, 3);

		Application::Update();
	}
};

PGame::Application* PGame::CreateApplication() {
	return new Game("SandBox");
}
