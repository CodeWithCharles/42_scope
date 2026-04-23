#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "system/Window.hpp"
#include "core/Config.hpp"

#include <stdexcept>

namespace Scop
{
	bool Window::isInitialized = false;
	void Window::initGlfw()
	{
		if (isInitialized) return;
		if (glfwInit() == GLFW_FALSE)
			throw std::runtime_error("Failed to initialize GLFW");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Config::OPENGL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Config::OPENGL_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef	__APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		isInitialized = true;
	}

	Window::Window(int width, int height, const std::string& title)
		: m_handle(nullptr),
		  m_width(width),
		  m_height(height)
	{
		initGlfw();

		m_handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (m_handle == nullptr)
		{
			glfwTerminate();
			isInitialized = false;
			throw std::runtime_error("Failed to create GLFW window");
		}

		makeContextCurrent();
		glfwSetWindowUserPointer(m_handle, this);
		glfwSetFramebufferSizeCallback(m_handle, Window::framebufferSizeCallback);

		setVSync(Config::START_WITH_VSYNC);
	}

	Window::~Window()
	{
		if (m_handle != nullptr)
		{
			glfwDestroyWindow(m_handle);
			m_handle = nullptr;
		}
		glfwTerminate();
		isInitialized = false;
	}

	bool Window::shouldClose() const
	{
		return glfwWindowShouldClose(m_handle) == GLFW_TRUE;
	}

	void Window::setShouldClose(bool value)
	{
		glfwSetWindowShouldClose(m_handle, value ? GLFW_TRUE : GLFW_FALSE);
	}

	void Window::swapBuffers()
	{
		glfwSwapBuffers(m_handle);
	}

	void Window::pollEvents()
	{
		glfwPollEvents();
	}

	void Window::makeContextCurrent()
	{
		glfwMakeContextCurrent(m_handle);
	}

	void Window::setVSync(bool enabled)
	{
		glfwSwapInterval(enabled ? 1 : 0);
	}

	GLFWwindow* Window::getHandle() const
	{
		return m_handle;
	}

	int Window::getWidth() const
	{
		return m_width;
	}

	int Window::getHeight() const
	{
		return m_height;
	}

	void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (instance != nullptr)
		{
			instance->m_width = width;
			instance->m_height = height;
		}
		glViewport(0, 0, width, height);
	}
}
