#pragma once

#include "Material.hpp"
#include "Mesh.hpp"

#include <string>
#include <vector>

namespace Scop
{
	struct ModelPart
	{
		std::string	name;
		std::string	materialName;
		Mesh*		mesh = nullptr;
	};

	class Model
	{
		private:
			std::vector<ModelPart>	m_parts;
			std::vector<Material>	m_materials;

		public:
			Model();
			~Model();

			Model(const Model&) = delete;
			Model& operator=(const Model&) = delete;

			Model(Model&&) = delete;
			Model& operator=(Model&&) = delete;

			void	draw() const;

			void	addPart(const ModelPart& part);
			void	addMaterial(const Material& material);

			const	std::vector<ModelPart>&	getParts() const;
			const	std::vector<Material>&	getMaterials() const;
	};
}
