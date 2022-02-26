#pragma once

#include "Core/Core.h"
#include "Core/Base.h"

extern Engine::Core* Engine::CreateApplication();

int main() {
	ENGINE_INFO("Engine - Version " << ENGINE_VERSION);
	ENGINE_DEBUG("DEBUG BUILD - You might experience performance impacts.");

	Engine::Core* app = Engine::CreateApplication();
	
	ENGINE_INFO("Initializing Application...");

	if (app->Init() == ENGINE_FAILURE) {
		ENGINE_ERROR("Failed to initialize Application!");
		return EXIT_FAILURE;
	}

	app->MainLoop();

	delete app;

	return EXIT_SUCCESS;
}