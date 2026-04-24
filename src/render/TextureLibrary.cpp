#include "render/TextureLibrary.hpp"

#include <string>
#include <vector>

namespace Scop
{
	TextureLibrary::TextureLibrary()
		: m_fallbackTexture(nullptr)
	{
	}

	TextureLibrary::~TextureLibrary()
	{
		clear();
	}

	void	TextureLibrary::initialize(const std::string& fallbackTexturePath)
	{
		clear();
		m_fallbackTexture = loadTexture(fallbackTexturePath);
	}

	void	TextureLibrary::preloadModelTextures(const Model& model)
	{
		const std::vector<ModelPart>& parts = model.getParts();

		for (std::size_t i = 0; i < parts.size(); ++i)
			getMaterialTextureForPart(model, parts[i]);
	}

	Texture*	TextureLibrary::getTextureForPart(
		const Model& model,
		const ModelPart& part,
		TextureSourceMode textureSourceMode)
	{
		if (textureSourceMode == TextureSourceMode::MaterialTexture)
			return getMaterialTextureForPart(model, part);

		return m_fallbackTexture;
	}

	Texture*	TextureLibrary::findLoadedTexture(const std::string& path) const
	{
		for (std::size_t i = 0; i < m_loadedTextures.size(); ++i)
		{
			if (m_loadedTextures[i].path == path)
				return m_loadedTextures[i].texture;
		}
		return nullptr;
	}

	Texture*	TextureLibrary::loadTexture(const std::string& path)
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

	Texture*	TextureLibrary::getMaterialTextureForPart(
		const Model& model,
		const ModelPart& part)
	{
		const Material* material = model.findMaterialByName(part.materialName);
		if (material == nullptr || material->diffuseTexturePath.empty())
			return m_fallbackTexture;

		const std::string& texturePath = material->diffuseTexturePath;

		if (texturePath.size() < 4
			|| texturePath.substr(texturePath.size() - 4) != ".ppm")
			return m_fallbackTexture;

		return loadTexture(texturePath);
	}

	void	TextureLibrary::clear()
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
	}
}
