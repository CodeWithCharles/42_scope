#pragma once

#include "system/Window.hpp"
#include "render/Shader.hpp"
#include "scene/Model.hpp"
#include "render/Texture.hpp"
#include "render/Camera.hpp"

#include "math/Mat4.hpp"
#include "math/Vec3.hpp"
#include <string>
#include <vector>

namespace Scop
{
	class App
	{
		struct LoadedTexture
		{
			std::string	path;
			Texture*	texture;
		};

		private:
			Window						m_window;
			bool						m_isRunning;
			Shader						*m_shader;
			Camera						*m_camera;
			Model						*m_model;
			Texture						*m_fallbackTexture;
			std::vector<LoadedTexture>	m_loadedTextures;
			float						m_textureBlend;
			bool						m_textureEnabled;
			bool						m_textureTogglePressed;
			float						m_rotationAngle;
			float						m_deltaTime;
			float						m_lastFrameTime;
			Math::Vec3					m_position;

		public:
			App();
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
			void		preloadModelTextures();

			Texture*	findLoadedTexture(const std::string& path) const;
			Texture*	loadTexture(const std::string& path);
			Texture*	getTextureForPart(const ModelPart& part);
	};
}
