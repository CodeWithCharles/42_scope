#pragma once

#include "render/RenderModes.hpp"
#include "render/Texture.hpp"
#include "scene/Model.hpp"

#include <string>
#include <vector>

namespace Scop
{
	class TextureLibrary
	{
		struct LoadedTexture
		{
			std::string	path;
			Texture*	texture;
		};

		private:
			Texture*					m_fallbackTexture;
			std::vector<LoadedTexture>	m_loadedTextures;

		public:
			TextureLibrary();
			~TextureLibrary();

			TextureLibrary(const TextureLibrary&) = delete;
			TextureLibrary& operator=(const TextureLibrary&) = delete;

			void	initialize(const std::string& fallbackTexturePath);
			void	preloadModelTextures(const Model& model);

			Texture*	getTextureForPart(
				const Model& model,
				const ModelPart& part,
				TextureSourceMode textureSourceMode);

		private:
			Texture*	findLoadedTexture(const std::string& path) const;
			Texture*	loadTexture(const std::string& path);
			Texture*	getMaterialTextureForPart(
				const Model& model,
				const ModelPart& part);
			void		clear();
	};
}
