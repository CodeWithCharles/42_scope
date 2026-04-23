#include "scene/Model.hpp"

namespace Scop
{
	Model::Model()
		:	m_minBounds({0.0f, 0.0f, 0.0f}),
			m_maxBounds({0.0f, 0.0f, 0.0f}),
			m_center({0.0f, 0.0f, 0.0f})
	{
	}

	Model::~Model()
	{
		for (std::size_t i = 0; i < m_parts.size(); ++i)
		{
			if (m_parts[i].mesh != nullptr)
			{
				delete m_parts[i].mesh;
				m_parts[i].mesh = nullptr;
			}
		}
	}

	void	Model::draw() const
	{
		for (std::size_t i = 0; i < m_parts.size(); ++i)
		{
			if (m_parts[i].mesh != nullptr)
				m_parts[i].mesh->draw();
		}
	}

	void	Model::addPart(const ModelPart& part)
	{
		m_parts.push_back(part);
		computeBounds();
	}

	void	Model::addMaterial(const Material& material)
	{
		m_materials.push_back(material);
	}

	void	Model::computeBounds()
	{
		bool	hasValidMesh = false;

		for (std::size_t i = 0; i < m_parts.size(); ++i)
		{
			if (m_parts[i].mesh == nullptr)
				continue;

			const Math::Vec3& partMin = m_parts[i].mesh->getMinBounds();
			const Math::Vec3& partMax = m_parts[i].mesh->getMaxBounds();

			if (!hasValidMesh)
			{
				m_minBounds = partMin;
				m_maxBounds = partMax;
				hasValidMesh = true;
				continue;
			}

			if (partMin.x < m_minBounds.x)
				m_minBounds.x = partMin.x;
			if (partMin.y < m_minBounds.y)
				m_minBounds.y = partMin.y;
			if (partMin.z < m_minBounds.z)
				m_minBounds.z = partMin.z;

			if (partMax.x > m_maxBounds.x)
				m_maxBounds.x = partMax.x;
			if (partMax.y > m_maxBounds.y)
				m_maxBounds.y = partMax.y;
			if (partMax.z > m_maxBounds.z)
				m_maxBounds.z = partMax.z;
		}

		if (!hasValidMesh)
		{
			m_minBounds = {0.0f, 0.0f, 0.0f};
			m_maxBounds = {0.0f, 0.0f, 0.0f};
			m_center = {0.0f, 0.0f, 0.0f};
			return;
		}

		m_center = {
			(m_minBounds.x + m_maxBounds.x) * 0.5f,
			(m_minBounds.y + m_maxBounds.y) * 0.5f,
			(m_minBounds.z + m_maxBounds.z) * 0.5f,
		};
	}

	const std::vector<ModelPart>&	Model::getParts() const
	{
		return m_parts;
	}

	const std::vector<Material>&	Model::getMaterials() const
	{
		return m_materials;
	}

	const Material*	Model::findMaterialByName(const std::string& name) const
	{
		for (std::size_t i = 0; i < m_materials.size(); ++i)
		{
			if (m_materials[i].name == name)
				return &m_materials[i];
		}
		return nullptr;
	}

	std::string	Model::findFirstDiffuseTexturePath() const
	{
		for (std::size_t i = 0; i < m_parts.size(); ++i)
		{
			const Material* material = findMaterialByName(m_parts[i].materialName);

			if (material != nullptr && !material->diffuseTexturePath.empty())
				return material->diffuseTexturePath;
		}
		return "";
	}

	const Math::Vec3&	Model::getMinBounds() const
	{
		return m_minBounds;
	}

	const Math::Vec3&	Model::getMaxBounds() const
	{
		return m_maxBounds;
	}

	const Math::Vec3&	Model::getCenter() const
	{
		return m_center;
	}

	Math::Vec3	Model::getSize() const
	{
		return {
			m_maxBounds.x - m_minBounds.x,
			m_maxBounds.y - m_minBounds.y,
			m_maxBounds.z - m_minBounds.z
		};
	}
}
