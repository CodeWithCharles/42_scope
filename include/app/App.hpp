#pragma once

#include "system/Window.hpp"
#include "render/Shader.hpp"
#include "scene/Model.hpp"
#include "render/Camera.hpp"
#include "app/AppOptions.hpp"
#include "render/RenderState.hpp"
#include "app/InputController.hpp"
#include "render/TextureLibrary.hpp"

#include "math/Mat4.hpp"
#include "math/Vec3.hpp"
#include <string>
#include <vector>

namespace Scop
{
	class App
	{
		private:
			AppOptions m_options;
			Window						m_window;
			bool						m_isRunning;
			Shader						*m_shader;
			Camera						*m_camera;
			Model						*m_model;
			TextureLibrary				m_textureLibrary;
			RenderState					m_renderState;
			InputController				m_inputController;
			float						m_rotationAngle;
			float						m_deltaTime;
			float						m_lastFrameTime;
			Math::Vec3					m_position;

		public:
			App(const AppOptions& options);
			~App();

			App(const App&) = delete;
			App& operator=(const App&) = delete;

			void	run();

		private:
			void		init();
			void		initGlad();
			void		configureOpenGL();
			void		processInput();
			void		update();
			void		render();
			void		initScene();
			void		cleanupScene();
			void		renderModelParts();

			float		getTextureBlendForRender() const;
			void		applyPolygonMode() const;
	};
}
