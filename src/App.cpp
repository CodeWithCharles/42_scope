#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "App.hpp"
#include "Config.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace Scope
{
	App::App()
		: m_window(
			Config::WINDOW_WIDTH,
			Config::WINDOW_HEIGHT,
			Config::WINDOW_TITLE),
		  m_isRunning(false),
		  m_shader(nullptr),
		  m_mesh(nullptr)
	{
	}

	App::~App()
	{
		cleanupScene();
	}

	void	App::run()
	{
		init();
		m_isRunning = true;

		while (m_isRunning && !m_window.shouldClose())
		{
			processInput();
			update();
			render();
			m_window.swapBuffers();
			m_window.pollEvents();
		}
	}

	void	App::init()
	{
		initGlad();
		configureOpenGL();
		initScene();
	}

	void	App::initGlad()
	{
		if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
			throw std::runtime_error("Failed to initialize GLAD");
	}

	void	App::configureOpenGL()
	{
		glViewport(0, 0, m_window.getWidth(), m_window.getHeight());
		glEnable(GL_DEPTH_TEST);
	}

	void	App::processInput()
	{
		if (glfwGetKey(m_window.getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			m_window.setShouldClose(true);
	}

	void	App::update()
	{
		// Phase 1: vide
	}

	void	App::render()
	{
		glClearColor(
			Config::CLEAR_RED,
			Config::CLEAR_GREEN,
			Config::CLEAR_BLUE,
			Config::CLEAR_ALPHA);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_shader->use();
		m_mesh->draw();
	}

	void	App::initScene()
	{
		const std::string	vertexShaderSource =
			"#version 410 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec3 aColor;\n"
			"\n"
			"out vec3 vColor;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	gl_Position = vec4(aPos, 1.0);\n"
			"	vColor = aColor;\n"
			"}\n";

		const std::string	fragmentShaderSource =
			"#version 410 core\n"
			"in vec3 vColor;\n"
			"\n"
			"out vec4 FragColor;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	FragColor = vec4(vColor, 1.0);\n"
			"}\n";

		const std::vector<Vertex>	vertices = {
			{{ 0.0f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.5f, 1.0f }},
			{{ -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }},
			{{ 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }}
		};

		const std::vector<unsigned int>	indices;

		m_shader = new Shader(vertexShaderSource, fragmentShaderSource);
		m_mesh = new Mesh(vertices, indices);
	}

	void	App::cleanupScene()
	{
		if (m_mesh != nullptr)
		{
			delete m_mesh;
			m_mesh = nullptr;
		}

		if (m_shader != nullptr)
		{
			delete m_shader;
			m_shader = nullptr;
		}
	}
}
