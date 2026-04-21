#include "ObjLoader.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace Scope
{
	Mesh*	ObjLoader::load(const std::string& path)
	{
		std::ifstream file(path.c_str());
		if (!file.is_open())
			throw std::runtime_error("Failed to open OBJ file: " + path);

		std::vector<Math::Vec3> positions;
		std::vector<unsigned int> indices;

		std::string line;
		while (std::getline(file, line))
		{
			if (line.empty())
				continue;

			std::istringstream lineStream(line);
			std::string prefix;
			lineStream >> prefix;

			if (prefix == "v")
			{
				Math::Vec3 position;
				lineStream >> position.x >> position.y >> position.z;
				positions.push_back(position);
			}
			else if (prefix == "f")
			{
				unsigned int a;
				unsigned int b;
				unsigned int c;

				lineStream >> a >> b >> c;

				if (a == 0 || b == 0 || c == 0)
					throw std::runtime_error("OBJ face indices must start at 1");

				indices.push_back(a - 1);
				indices.push_back(b - 1);
				indices.push_back(c - 1);
			}
		}

		std::vector<Vertex> vertices;
		vertices.reserve(positions.size());

		for (std::size_t i = 0; i < positions.size(); ++i)
		{
			Vertex vertex;
			vertex.position = positions[i];
			vertex.color = { 1.0f, 1.0f, 1.0f };
			vertex.uv = { 0.0f, 0.0f };
			vertices.push_back(vertex);
		}

		return new Mesh(vertices, indices);
	}
}
