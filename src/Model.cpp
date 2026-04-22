#include "Model.hpp"

namespace Scop
{
	Model::Model()
	{}

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
	}

	void	Model::addMaterial(const Material& material)
	{
		m_materials.push_back(material);
	}

	const	std::vector<ModelPart>&	Model::getParts() const
	{
		return m_parts;
	}

	const	std::vector<Material>&	Model::getMaterials() const
	{
		return m_materials;
	}
}
