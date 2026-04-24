#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "app/App.hpp"
#include "core/Config.hpp"
#include "io/ObjLoader.hpp"
#include "render/ModelTransformBuilder.hpp"

#include <stdexcept>

namespace Scop
{
	App::App(const AppOptions& options)
		: m_options(options),
		  m_window(
			Config::WINDOW_WIDTH,
			Config::WINDOW_HEIGHT,
			Config::WINDOW_TITLE),
		  m_isRunning(false),
		  m_shader(nullptr),
		  m_camera(nullptr),
		  m_model(nullptr),
		  m_textureLibrary(),
		  m_renderState(),
		  m_rotationAngle(0.0f),
		  m_deltaTime(0.0f),
		  m_lastFrameTime(0.0f),
		  m_position({0.0f, 0.0f, 0.0f})
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
		glPointSize(1.5f);
		glEnable(GL_DEPTH_TEST);
	}

	void	App::processInput()
	{
		if (glfwGetKey(m_window.getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			m_window.setShouldClose(true);

		m_inputController.update(
			m_window.getHandle(),
			m_deltaTime,
			m_position,
			m_renderState);
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

		Math::Mat4 mvp = ModelTransformBuilder::buildMvpMatrix(
			*m_model,
			*m_camera,
			m_position,
			m_rotationAngle,
			Config::MODEL_IMPORT_ROTATION_X_RADIANS,
			m_window.getWidth(),
			m_window.getHeight());


		m_shader->use();
		m_shader->setMat4("uMVP", mvp);
		m_shader->setInt("uTexture", 0);
		m_shader->setFloat("uTextureBlend", getTextureBlendForRender());

		applyPolygonMode();
		renderModelParts();
	}

	void	App::initScene()
	{
		const std::string	vertexShaderPath = "assets/shaders/basic.vert";
		const std::string	fragmentShaderPath = "assets/shaders/basic.frag";
		const std::string&	modelPath = m_options.modelPath;
		const std::string&	fallbackTexturePath = m_options.fallbackTexturePath.empty()
			? std::string(Config::TEXTURE_PATH)
			: m_options.fallbackTexturePath;

		m_shader = new Shader(vertexShaderPath, fragmentShaderPath);
		m_camera = new Camera(
			Config::CAMERA_DISTANCE,
			Config::PROJECTION_FOV_RADIANS,
			Config::PROJECTION_NEAR_PLANE,
			Config::PROJECTION_FAR_PLANE);

		m_model = ObjLoader::load(modelPath);
		m_textureLibrary.initialize(fallbackTexturePath);
		m_textureLibrary.preloadModelTextures(*m_model);
	}

	void	App::cleanupScene()
	{
		if (m_model != nullptr)
		{
			delete m_model;
			m_model = nullptr;
		}

		if (m_shader != nullptr)
		{
			delete m_shader;
			m_shader = nullptr;
		}

		if (m_camera != nullptr)
		{
			delete m_camera;
			m_camera = nullptr;
		}
	}

	float	App::getTextureBlendForRender() const
	{
		if (m_renderState.textureSourceMode == TextureSourceMode::PolygonColor)
			return 0.0f;
		return m_renderState.textureBlend;
	}

	void	App::applyPolygonMode() const
	{

		GLenum mode = m_renderState.polygonMode == PolygonMode::Wireframe
			? GL_LINE : m_renderState.polygonMode == PolygonMode::Point
			? GL_POINT : GL_FILL;
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}

	void	App::renderModelParts()
	{
		const std::vector<ModelPart>& parts = m_model->getParts();

		for (std::size_t i = 0; i < parts.size(); ++i)
		{
			Texture* texture = m_textureLibrary.getTextureForPart(
				*m_model,
				parts[i],
				m_renderState.textureSourceMode);

			glActiveTexture(GL_TEXTURE0);
			texture->bind();

			if (parts[i].mesh != nullptr)
				parts[i].mesh->draw();
		}
	}
}
