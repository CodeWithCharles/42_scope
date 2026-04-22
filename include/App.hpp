#pragma once

#include "Window.hpp"
#include "Shader.hpp"
#include "Model.hpp"

#include "math/Mat4.hpp"
#include "math/Vec3.hpp"

namespace Scop
{
	class App
	{
		private:
			Window		m_window;
			bool		m_isRunning;
			Shader		*m_shader;
			Model		*m_model;
			float		m_rotationAngle;
			float		m_deltaTime;
			float		m_lastFrameTime;
			Math::Vec3	m_position;

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
