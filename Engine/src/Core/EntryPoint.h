#pragma once

#include "Core/Application.h"
#include "Core/Base.h"

extern Engine::Application* Engine::CreateApplication();

int main() {
	pgInfo("Portal Game - Version " << PG_VERSION);
	pgDebug("DEBUG BUILD - You might experience performance impacts.");

	Engine::Application* app = Engine::CreateApplication();
	
	bool didInit = app->Init();

	if (didInit == PG_FAILURE) {
		pgError("Failed to initialize Application!");
		return EXIT_FAILURE;
	}

	while (app->IsRunning()) {
		app->Update();
	}

	delete app;

	return EXIT_SUCCESS;
}