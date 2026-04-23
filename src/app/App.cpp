#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "app/App.hpp"
#include "core/Config.hpp"
#include "io/ObjLoader.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace Scop
{
	App::App()
		: m_window(
			Config::WINDOW_WIDTH,
			Config::WINDOW_HEIGHT,
			Config::WINDOW_TITLE),
		  m_isRunning(false),
		  m_shader(nullptr),
		  m_camera(nullptr),
		  m_model(nullptr),
		  m_fallbackTexture(nullptr),
		  m_textureBlend(1.0f),
		  m_textureEnabled(true),
		  m_textureTogglePressed(false),
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
		glEnable(GL_DEPTH_TEST);
	}

	void	App::processInput()
	{
		if (glfwGetKey(m_window.getHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		m_window.setShouldClose(true);

		float translationStep = Config::TRANSLATION_SPEED_UNITS_PER_SECOND * m_deltaTime;

		if (glfwGetKey(m_window.getHandle(), GLFW_KEY_A) == GLFW_PRESS)
			m_position.x += translationStep;
		if (glfwGetKey(m_window.getHandle(), GLFW_KEY_D) == GLFW_PRESS)
			m_position.x -= translationStep;

		if (glfwGetKey(m_window.getHandle(), GLFW_KEY_S) == GLFW_PRESS)
			m_position.y += translationStep;
		if (glfwGetKey(m_window.getHandle(), GLFW_KEY_W) == GLFW_PRESS)
			m_position.y -= translationStep;

		if (glfwGetKey(m_window.getHandle(), GLFW_KEY_Q) == GLFW_PRESS)
			m_position.z -= translationStep;
		if (glfwGetKey(m_window.getHandle(), GLFW_KEY_E) == GLFW_PRESS)
			m_position.z += translationStep;

		int	textureToggleState = glfwGetKey(m_window.getHandle(), GLFW_KEY_T);

		if (textureToggleState == GLFW_PRESS && !m_textureTogglePressed)
		{
			m_textureEnabled = !m_textureEnabled;
			m_textureTogglePressed = true;
		}
		else if (textureToggleState == GLFW_RELEASE)
		{
			m_textureTogglePressed = false;
		}
	}

	void	App::update()
	{
		if (m_deltaTime < 0.0f)
			return;
		if (m_deltaTime > 0.1f)
			m_deltaTime = 0.1f;

		m_rotationAngle += Config::ROTATION_SPEED_RADIANS_PER_SECOND * m_deltaTime;

		float	targetBlend = m_textureEnabled ? 1.0f : 0.0f;
		float	blendStep = 2.0f * m_deltaTime;

		if (m_textureBlend < targetBlend)
		{
			m_textureBlend += blendStep;
			if (m_textureBlend > targetBlend)
				m_textureBlend = targetBlend;
		}
		else if (m_textureBlend > targetBlend)
		{
			m_textureBlend -= blendStep;
			if (m_textureBlend < targetBlend)
				m_textureBlend = targetBlend;
		}
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

	Texture*	App::getTextureForPart(const ModelPart& part)
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

	void	App::preloadModelTextures()
	{
		const std::vector<ModelPart>& parts = m_model->getParts();

		for (std::size_t i = 0; i < parts.size(); ++i)
			getTextureForPart(parts[i]);
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
		m_shader->setFloat("uTextureBlend", m_textureBlend);
		const std::vector<ModelPart>& parts = m_model->getParts();

		for (std::size_t i = 0; i < parts.size(); ++i)
		{
			Texture* texture = getTextureForPart(parts[i]);

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

		m_shader = new Shader(vertexShaderPath, fragmentShaderPath);
		m_camera = new Camera(
			Config::CAMERA_DISTANCE,
			Config::PROJECTION_FOV_RADIANS,
			Config::PROJECTION_NEAR_PLANE,
			Config::PROJECTION_FAR_PLANE);
		m_model = ObjLoader::load(Config::MODEL_PATH);

		m_fallbackTexture = loadTexture(Config::TEXTURE_PATH);
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
}
