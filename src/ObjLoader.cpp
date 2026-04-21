#include "ObjLoader.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace Scop
{
	Mesh*	ObjLoader::load(const std::string& path)
	{
		std::ifstream file(path.c_str());
		if (!file.is_open())
			throw std::runtime_error("Failed to open OBJ file: " + path);

		std::vector<Math::Vec3> positions;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		const Math::Vec3 colorPalette[] = {
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 1.0f},
			{1.0f, 0.5f, 0.0f},
			{0.7f, 0.7f, 0.7f}
		};

		std::size_t faceIndex = 0;
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

				unsigned int indexA = a - 1;
				unsigned int indexB = b - 1;
				unsigned int indexC = c - 1;

				if (indexA >= positions.size() || indexB >= positions.size() || indexC >= positions.size())
					throw std::runtime_error("OBJ face index out of bounds");

				const std::size_t paletteSize = sizeof(colorPalette) / sizeof(colorPalette[0]);
				Math::Vec3 faceColor = colorPalette[faceIndex % paletteSize];

				unsigned int indexOffset = static_cast<unsigned int>(vertices.size());

				Vertex v0;
				v0.position = positions[indexA];
				v0.color = faceColor;
				v0.uv = {0.0f, 0.0f};

				Vertex v1;
				v1.position = positions[indexB];
				v1.color = faceColor;
				v1.uv = {0.0f, 0.0f};

				Vertex v2;
				v2.position = positions[indexC];
				v2.color = faceColor;
				v2.uv = {0.0f, 0.0f};

				vertices.push_back(v0);
				vertices.push_back(v1);
				vertices.push_back(v2);

				indices.push_back(indexOffset + 0);
				indices.push_back(indexOffset + 1);
				indices.push_back(indexOffset + 2);

				++faceIndex;
			}
		}

		return new Mesh(vertices, indices);
	}
}
