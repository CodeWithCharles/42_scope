#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "App.hpp"
#include "Config.hpp"
#include "ObjLoader.hpp"

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
		  m_mesh(nullptr),
		  m_rotationAngle(0.0f)
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
		m_lastFrameTime = static_cast<float>(glfwGetTime());

		while (m_isRunning && !m_window.shouldClose())
		{
			float currentFrameTime = static_cast<float>(glfwGetTime());
			m_deltaTime = currentFrameTime - m_lastFrameTime;
			m_lastFrameTime = currentFrameTime;

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
		if (m_deltaTime < 0.0f)
			return;
		if (m_deltaTime > 0.1f)
			m_deltaTime = 0.1f;
		m_rotationAngle += Config::ROTATION_SPEED_RADIANS_PER_SECOND * m_deltaTime;
	}

	void	App::render()
	{
		glClearColor(
			Config::CLEAR_RED,
			Config::CLEAR_GREEN,
			Config::CLEAR_BLUE,
			Config::CLEAR_ALPHA);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Math::Mat4 model = Math::Mat4::rotationY(m_rotationAngle);
		Math::Mat4 view = Math::Mat4::translation({0.0f, 0.0f, -Config::CAMERA_DISTANCE});

		float aspectRatio = static_cast<float>(m_window.getWidth())
			/ static_cast<float>(m_window.getHeight());

		Math::Mat4 projection = Math::Mat4::perspective(
			Config::PROJECTION_FOV_RADIANS,
			aspectRatio,
			Config::PROJECTION_NEAR_PLANE,
			Config::PROJECTION_FAR_PLANE);

		Math::Mat4 mvp = projection * view * model;

		m_shader->use();
		m_shader->setMat4("uMVP", mvp);
		m_mesh->draw();
	}

	void	App::initScene()
	{
		const std::string	vertexShaderPath = "assets/shaders/basic.vert";
		const std::string	fragmentShaderPath = "assets/shaders/basic.frag";

		m_shader = new Shader(vertexShaderPath, fragmentShaderPath);
		m_mesh = ObjLoader::load(Config::MODEL_PATH);
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
