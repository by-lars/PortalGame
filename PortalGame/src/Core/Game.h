#ifndef _PG_CORE_GAME_H_
#define _PG_CORE_GAME_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "Core/Base.h"
#include "OpenGL/Shader.h"
#include "OpenGL/SimpleBuffer.h"



namespace PGame {
	class Game {
	public:
		Game(int windowWidth, int windowHeight, const std::string &windowTitle);
		~Game();

		bool ShouldRun();

		bool Init();
		void Loop();

		float vertices[18] = {
			// positions         
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f , 0.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
		};


		Ref<GL::SimpleBuffer> buffer;
		Ref<GL::Shader> shader;
	public:
		GLFWwindow* window;
		std::string title;
		int width;
		int height;
	};
}

#endif
