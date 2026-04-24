#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "app/App.hpp"
#include "core/Config.hpp"
#include "io/ObjLoader.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace
{
	float	clampFloat(float value, float minValue, float maxValue)
	{
		if (value < minValue)
			return minValue;
		if (value > maxValue)
			return maxValue;
		return value;
	}
}
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
		  m_fallbackTexture(nullptr),
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
		glPointSize(1.1f);
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

	Texture*	App::findLoadedTexture(const std::string& path) const
	{
		for (std::size_t i = 0; i < m_loadedTextures.size(); ++i)
		{
			if (m_loadedTextures[i].path == path)
				return m_loadedTextures[i].texture;
		}
		return nullptr;
	}

	Texture*	App::loadTexture(const std::string& path)
	{
		Texture*	existingTexture = findLoadedTexture(path);

		if (existingTexture != nullptr)
			return existingTexture;

		LoadedTexture loadedTexture;
		loadedTexture.path = path;
		loadedTexture.texture = new Texture(path);

		m_loadedTextures.push_back(loadedTexture);
		return loadedTexture.texture;
	}

	void	App::preloadModelTextures()
	{
		const std::vector<ModelPart>& parts = m_model->getParts();

		for (std::size_t i = 0; i < parts.size(); ++i)
			getMaterialTextureForPart(parts[i]);
	}

	void	App::render()
	{
		glClearColor(
			Config::CLEAR_RED,
			Config::CLEAR_GREEN,
			Config::CLEAR_BLUE,
			Config::CLEAR_ALPHA);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Math::Vec3 center = m_model->getCenter();
		Math::Vec3 size = m_model->getSize();

		float maxExtent = size.x;

		if (size.y > maxExtent)
			maxExtent = size.y;
		if (size.z > maxExtent)
			maxExtent = size.z;

		float scaleFactor = 1.0f;
		if (maxExtent > 0.0f)
			scaleFactor = 1.0f / maxExtent;

		Math::Mat4 toOrigin = Math::Mat4::translation({
			-center.x,
			-center.y,
			-center.z
		});

		Math::Mat4 scale = Math::Mat4::scale({scaleFactor, scaleFactor, scaleFactor});
		Math::Mat4 importRotation = Math::Mat4::rotationX(Config::MODEL_IMPORT_ROTATION_X_RADIANS);
		Math::Mat4 animationRotation = Math::Mat4::rotationY(m_rotationAngle);
		Math::Mat4 rotation = animationRotation * importRotation;
		// Math::Mat4 backToCenter = Math::Mat4::translation(center);
		Math::Mat4 translation = Math::Mat4::translation(m_position);

		Math::Mat4 model = translation * rotation * scale * toOrigin;
		Math::Mat4 view = m_camera->getViewMatrix();

		Math::Mat4 projection = m_camera->getProjectionMatrix(m_window.getWidth(),
			m_window.getHeight());

		Math::Mat4 mvp = projection * view * model;

		m_shader->use();
		m_shader->setMat4("uMVP", mvp);
		m_shader->setInt("uTexture", 0);
		m_shader->setFloat("uTextureBlend", getTextureBlendForRender());
		applyPolygonMode();
		const std::vector<ModelPart>& parts = m_model->getParts();

		for (std::size_t i = 0; i < parts.size(); ++i)
		{
			Texture* texture = selectTextureForPart(parts[i]);

			glActiveTexture(GL_TEXTURE0);
			texture->bind();

			if (parts[i].mesh != nullptr)
				parts[i].mesh->draw();
		}
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
		m_fallbackTexture = loadTexture(fallbackTexturePath);

		preloadModelTextures();
	}

	void	App::cleanupScene()
	{
		for (std::size_t i = 0; i < m_loadedTextures.size(); ++i)
		{
			if (m_loadedTextures[i].texture != nullptr)
			{
				delete m_loadedTextures[i].texture;
				m_loadedTextures[i].texture = nullptr;
			}
		}
		m_loadedTextures.clear();
		m_fallbackTexture = nullptr;
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

	Texture*	App::getMaterialTextureForPart(const ModelPart& part)
	{
		const Material*	material = m_model->findMaterialByName(part.materialName);

		if (material == nullptr || material->diffuseTexturePath.empty())
			return m_fallbackTexture;

		const std::string& texturePath = material->diffuseTexturePath;

		if (texturePath.size() < 4
			|| texturePath.substr(texturePath.size() - 4) != ".ppm")
			return m_fallbackTexture;

		return loadTexture(texturePath);
	}

	Texture*	App::selectTextureForPart(const ModelPart& part)
	{
		if (m_renderState.textureSourceMode == TextureSourceMode::MaterialTexture)
			return getMaterialTextureForPart(part);
		return m_fallbackTexture;
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
}
