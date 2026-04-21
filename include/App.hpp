#pragma once

#include "Window.hpp"
//! A RETIRER UNE FOIS LE TEST FAIT
#include "Shader.hpp"

namespace Scope
{
	class App
	{
		private:
			Window	m_window;
			bool	m_isRunning;
			// //! A RETIRER UNE FOIS LE TEST FAIT
			// Shader*	m_shader;
			// unsigned int	m_vao;
			// unsigned int	m_vbo;

		public:
			App();
			~App();

			App(const App&) = delete;
			App& operator=(const App&) = delete;

			void	run();

		private:
			void	init();
			void	initGlad();
			void	configureOpenGL();
			void	processInput();
			void	update();
			void	render();
			// //! A RETIRER UNE FOIS LE TEST FAIT
			// void	initDemoTriangle();
			// void	cleanupDemoTriangle();
	};
}
