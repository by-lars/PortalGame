#include "Core/Base.h"
#include "Core/Game.h"

int main() {
	pgInfo("Portal Game - Version " << PG_VERSION);
	pgDebug("This is a debug build.");
	pgDebug("You might experience performance impacts.");
	
	PGame::Game game(800, 600, "Portal Game");

	if (game.Init() == PG_FAILURE) {
		return EXIT_FAILURE;
	}

	while (game.ShouldRun()) {
		game.Loop();
	}

	return EXIT_SUCCESS;
}