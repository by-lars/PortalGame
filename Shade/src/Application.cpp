#include "Core/EntryPoint.h"

#include "OpenGL/Shader.h"
#include "OpenGL/SimpleBuffer.h"

#include <fstream>

using namespace PGame;

class Game : public Application {
public:
	Game(const std::string& name) : Application(name) {
	
	}

	virtual bool Init() override {
		pgInfo("Init");
		
		return PG_SUCCESS;
	}

	virtual void Update() override {
	
		Application::Update();
	}
};

PGame::Application* PGame::CreateApplication() {
	return new Game("Shade");
}
