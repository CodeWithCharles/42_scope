#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "App.hpp"
#include "Config.hpp"

#include <stdexcept>

namespace Scope
{
	App::App()
		: m_window(
			Config::WINDOW_WIDTH,
			Config::WINDOW_HEIGHT,
			Config::WINDOW_TITLE),
		  m_isRunning(false)
		// //! A RETIRER UNE FOIS LE TEST FAIT
		// m_shader(nullptr),
		// m_vao(0),
		// m_vbo(0)
	{
	}

	App::~App()
	{
		// //! A RETIRER UNE FOIS LE TEST FAIT
		// cleanupDemoTriangle();
	}

	void App::run()
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

	void App::init()
	{
		initGlad();
		configureOpenGL();
		// //! A RETIRER UNE FOIS LE TEST FAIT
		// initDemoTriangle();
	}

	void App::initGlad()
	{
		if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
			throw std::runtime_error("Failed to initialize GLAD");
	}

	void App::configureOpenGL()
	{
		glViewport(0, 0, m_window.getWidth(), m_window.getHeight());
		glEnable(GL_DEPTH_TEST);
	}

	void App::processInput()
	{
		if (glfwGetKey(m_window.getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			m_window.setShouldClose(true);
	}

	void App::update()
	{
		// Phase 1: vide
	}

	void App::render()
	{
		glClearColor(
			Config::CLEAR_RED,
			Config::CLEAR_GREEN,
			Config::CLEAR_BLUE,
			Config::CLEAR_ALPHA);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// //! A RETIRER UNE FOIS LE TEST FAIT
		// m_shader->use();
		// glBindVertexArray(m_vao);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0);
	}

	// void App::initDemoTriangle()
	// {
	// 	//! A RETIRER UNE FOIS LE TEST FAIT
	// 	const std::string vertexShaderSource =
	// 		"#version 410 core\n"
	// 		"layout (location = 0) in vec3 aPos;\n"
	// 		"\n"
	// 		"void main()\n"
	// 		"{\n"
	// 		"    gl_Position = vec4(aPos, 1.0);\n"
	// 		"}\n";

	// 	const std::string fragmentShaderSource =
	// 		"#version 410 core\n"
	// 		"out vec4 FragColor;\n"
	// 		"\n"
	// 		"void main()\n"
	// 		"{\n"
	// 		"    FragColor = vec4(0.1, 0.2, 0.8, 1.0);\n"
	// 		"}\n";

	// 	const float vertices[] = {
	// 		0.0f,  0.5f, 0.0f,
	// 		-0.5f, -0.5f, 0.0f,
	// 		0.5f, -0.5f, 0.0f
	// 	};

	// 	m_shader = new Shader(vertexShaderSource, fragmentShaderSource);

	// 	glGenVertexArrays(1, &m_vao);
	// 	glGenBuffers(1, &m_vbo);

	// 	glBindVertexArray(m_vao);

	// 	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	// 	glEnableVertexAttribArray(0);

	// 	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 	glBindVertexArray(0);
	// }

	// void App::cleanupDemoTriangle()
	// {
	// 	if (m_vbo != 0)
	// 	{
	// 		glDeleteBuffers(1, &m_vbo);
	// 		m_vbo = 0;
	// 	}

	// 	if (m_vao != 0)
	// 	{
	// 		glDeleteVertexArrays(1, &m_vao);
	// 		m_vao = 0;
	// 	}

	// 	if (m_shader != nullptr)
	// 	{
	// 		delete m_shader;
	// 		m_shader = nullptr;
	// 	}
	// }

}
