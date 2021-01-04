#include "Core/Game.h"
#include "Core/Base.h"

#include "OpenGL/Shader.h"

#include <fstream>
#include <cmath>

namespace PGame {
	void APIENTRY OnOpenGLDebugMessage(GLenum source, GLenum type, unsigned int id, GLenum severity,
		GLsizei length, const char* message, const void* userParam) {
		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH:			pgError(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:			pgError(message); return;
			case GL_DEBUG_SEVERITY_LOW:				pgWarn(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION:	pgInfo(message); return;
			default:								pgInfo(message); return;
		}
	}


	Game::Game(int windowWidth, int windowHeight, const std::string& windowTitle)
		: width(windowWidth), height(windowHeight), title(windowTitle) 
	{}

	Game::~Game() {
	}

	bool Game::ShouldRun() {
		return !glfwWindowShouldClose(window);
	}

	bool Game::Init() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	#endif 

		window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (window == NULL) {
			pgError("Could not initialize window");
			glfwTerminate();
			return PG_FAILURE;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			pgError("Could not load OpenGL extensions");
			glfwTerminate();
			return PG_FAILURE;
		}

		glViewport(0, 0, width, height);

	#ifdef _DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OnOpenGLDebugMessage, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	#endif

		std::cout << std::endl;
		pgInfo("System Information");
		pgInfo("	GL Renderer:  " << glGetString(GL_RENDERER));
		pgInfo("	GL Version:   " << glGetString(GL_VERSION));
		pgInfo("	GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION));
		std::cout << std::endl;

		std::ifstream file("assets/shader/testshader.glsl");

		if (!file) {
			pgError("Coudl not open file :(");
		}
		file.seekg(0, std::ios::end);

		size_t size = file.tellg();
		std::string source(size, ' ');

		file.seekg(0, std::ios::beg);
		file.read(&source[0], size);


		shader = CreateRef<GL::Shader>();
		shader->Load(source);
		shader->Use();

		buffer = CreateRef<GL::SimpleBuffer>(sizeof(vertices));
		buffer->Upload(vertices, sizeof(vertices));

		buffer->SetFormat({ GL::BufferElement(GL_FLOAT, 3), GL::BufferElement(GL_FLOAT, 3) });

		buffer->Bind();

		return PG_SUCCESS;
	}

	void Game::Loop() {
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		vertices[0] = cosf(glfwGetTime());
		vertices[4] = tanf(glfwGetTime());
		vertices[7] = sinf(glfwGetTime());
		buffer->Upload(vertices, sizeof(vertices));

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}