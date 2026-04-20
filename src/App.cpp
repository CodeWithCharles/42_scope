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
	{
	}

	App::~App()
	{
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
	}
}
