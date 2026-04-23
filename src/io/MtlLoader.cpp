#include "io/MtlLoader.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace Scop
{
	std::vector<Material>	MtlLoader::load(const std::string& path)
	{
		std::ifstream file(path.c_str());
		if (!file.is_open())
			throw std::runtime_error("Failed to open MTL file: " + path);

		std::vector<Material>	materials;
		Material	currentMaterial;
		bool	hasCurrentMaterial = false;
		std::string	line;

		while (std::getline(file, line))
		{
			if (line.empty())
				continue;

			std::istringstream	lineStream(line);
			std::string	prefix;
			lineStream >> prefix;

			if (prefix.empty() || prefix[0] == '#')
				continue;

			if (prefix == "newmtl")
			{
				if (hasCurrentMaterial)
					materials.push_back(currentMaterial);

				currentMaterial = Material();
				lineStream >> currentMaterial.name;
				hasCurrentMaterial = true;
			}
			else if (!hasCurrentMaterial)
			{
				continue;
			}
			else if (prefix == "Ka")
			{
				lineStream >> currentMaterial.ambient.x
					>> currentMaterial.ambient.y
					>> currentMaterial.ambient.z;
			}
			else if (prefix == "Kd")
			{
				lineStream >> currentMaterial.diffuse.x
					>> currentMaterial.diffuse.y
					>> currentMaterial.diffuse.z;
			}
			else if (prefix == "Ks")
			{
				lineStream >> currentMaterial.specular.x
					>> currentMaterial.specular.y
					>> currentMaterial.specular.z;
			}
			else if (prefix == "Ns")
			{
				lineStream >> currentMaterial.shininess;
			}
			else if (prefix == "d")
			{
				lineStream >> currentMaterial.opacity;
			}
			else if (prefix == "illum")
			{
				lineStream >> currentMaterial.illuminationModel;
			}
			else if (prefix == "map_Kd")
			{
				lineStream >> currentMaterial.diffuseTexturePath;
			}
		}

		if (hasCurrentMaterial)
			materials.push_back(currentMaterial);

		return materials;
	}
}
