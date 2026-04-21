#pragma once

#include "Window.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

namespace Scope
{
	class App
	{
		private:
			Window	m_window;
			bool	m_isRunning;
			Shader	*m_shader;
			Mesh	*m_mesh;

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
			void	initScene();
			void	cleanupScene();
	};
}
