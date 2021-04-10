#pragma once

#include "Core/Application.h"
#include "Core/Base.h"

extern PGame::Application* PGame::CreateApplication();

int main() {
	pgInfo("Portal Game - Version " << PG_VERSION);
	pgDebug("This is a debug build.");
	pgDebug("You might experience performance impacts.");

	PGame::Application* app = PGame::CreateApplication();
	
	bool didInit = app->Init();

	if (didInit == PG_FAILURE) {
		pgError("Failed to initialize Application!");
		return EXIT_FAILURE;
	}

	while (app->IsRunning) {
		app->Update();
	}

	delete app;

	return EXIT_SUCCESS;
}