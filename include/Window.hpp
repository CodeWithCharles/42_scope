#pragma once

#include <string>

struct GLFWwindow;

namespace Scope
{
	class Window
	{
		private:
			GLFWwindow*	m_handle;
			int			m_width;
			int			m_height;
			static bool	isInitialized;

		public:
			Window(int width, int height, const std::string& title);
			~Window();

			Window(const Window&) = delete;
			Window& operator=(const Window&) = delete;

			Window(Window&&) = delete;
			Window&	operator=(Window&&) = delete;

			bool		shouldClose() const;
			void		setShouldClose(bool value);
			void		swapBuffers();
			void		pollEvents();
			void		makeContextCurrent();
			void		setVSync(bool enabled);
			GLFWwindow*	getHandle() const;
			int			getWidth() const;
			int			getHeight() const;

		private:
			static void	initGlfw();
			static void	framebufferSizeCallback(GLFWwindow* window, int width, int height);
	};
}
