#pragma once

#include "Window.hpp"

namespace Scope
{
	class App
	{
		private:
			Window	m_window;
			bool	m_isRunning;

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
	};
}
