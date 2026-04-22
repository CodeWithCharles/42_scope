#include "ObjLoader.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
	struct FaceVertex
	{
		int	positionIndex;
		int	textureIndex;
		int	normalIndex;
	};

	std::vector<std::string>	split(const std::string& value, char delimiter)
	{
		std::vector<std::string>	parts;
		std::string					current;

		for (std::size_t i = 0; i < value.size(); ++i)
		{
			if (value[i] == delimiter)
			{
				parts.push_back(current);
				current.clear();
			}
			else
				current	+= value[i];
		}
		parts.push_back(current);
		return parts;
	}

	int	parseObjIndex(const std::string& token, std::size_t currentCount)
	{
		if (token.empty())
			throw std::runtime_error("OBJ index token is empty");

		int index = std::stoi(token);
		if (index == 0)
			throw std::runtime_error("OBJ indices must start at 1");

		if (index > 0)
			return index - 1;

		int resolvedIndex = static_cast<int>(currentCount) + index;
		if (resolvedIndex < 0)
			throw std::runtime_error("OBJ negative index is out of bounds");

		return resolvedIndex;
	}

	FaceVertex	parseFaceVertexToken(const std::string& token, std::size_t positionsCount, std::size_t texCoordsCount, std::size_t normalsCount)
	{
		FaceVertex result = {-1, -1, -1};
		std::vector<std::string> parts = split(token, '/');

		if (parts.empty() || parts[0].empty())
			throw std::runtime_error("OBJ face token is missing a position");

		result.positionIndex = parseObjIndex(parts[0], positionsCount);

		if (parts.size() > 1 && !parts[1].empty())
			result.textureIndex = parseObjIndex(parts[1], texCoordsCount);

		if (parts.size() > 2 && !parts[2].empty())
			result.normalIndex = parseObjIndex(parts[2], normalsCount);

		return result;
	}
}

namespace Scop
{
	Mesh*	ObjLoader::load(const std::string& path)
	{
		std::ifstream file(path.c_str());
		if (!file.is_open())
			throw std::runtime_error("Failed to open OBJ file: " + path);

		std::vector<Math::Vec3>	positions;
		std::vector<Math::Vec2>	texCoords;
		std::vector<Math::Vec3>	normals;
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

			if (prefix == "#")
			{
				continue;
			}
			else if (prefix == "v")
			{
				Math::Vec3 position;
				lineStream >> position.x >> position.y >> position.z;
				positions.push_back(position);
			}
			else if (prefix == "vt")
			{
				Math::Vec2	texCoord;
				lineStream >> texCoord.x >> texCoord.y;
				texCoords.push_back(texCoord);
			}
			else if (prefix == "vn")
			{
				Math::Vec3	normal;
				lineStream >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);
			}
			else if (prefix == "f")
			{
				std::vector<std::string> faceTokens;
				std::string token;

				while (lineStream >> token)
					faceTokens.push_back(token);

				if (faceTokens.size() < 3)
					throw std::runtime_error("OBJ faces must contain at least 3 vertices");

				std::vector<FaceVertex> faceVertices;
				faceVertices.reserve(faceTokens.size());

				for (std::size_t i = 0; i < faceTokens.size(); ++i)
				{
					FaceVertex faceVertex = parseFaceVertexToken(
						faceTokens[i],
						positions.size(),
						texCoords.size(),
						normals.size());

					if (faceVertex.positionIndex < 0)
						throw std::runtime_error("OBJ face position index is invalid");

					if (static_cast<std::size_t>(faceVertex.positionIndex) >= positions.size())
						throw std::runtime_error("OBJ face index out of bounds");

					if (faceVertex.textureIndex >= 0 && static_cast<std::size_t>(faceVertex.textureIndex) >= texCoords.size())
						throw std::runtime_error("OBJ texture coordinate index out of bounds");

					if (faceVertex.normalIndex >= 0 && static_cast<std::size_t>(faceVertex.normalIndex) >= normals.size())
						throw std::runtime_error("OBJ normal index out of bounds");

					faceVertices.push_back(faceVertex);
				}

				const std::size_t paletteSize = sizeof(colorPalette) / sizeof(colorPalette[0]);
				Math::Vec3 faceColor = colorPalette[faceIndex % paletteSize];

				for (std::size_t i = 1; i + 1 < faceVertices.size(); ++i)
				{
					const FaceVertex& a = faceVertices[0];
					const FaceVertex& b = faceVertices[i];
					const FaceVertex& c = faceVertices[i + 1];

					unsigned int indexOffset = static_cast<unsigned int>(vertices.size());


					Vertex v0;
					v0.position = positions[a.positionIndex];
					v0.color = faceColor;
					if (a.textureIndex >= 0)
						v0.uv = texCoords[a.textureIndex];
					else
						v0.uv = {0.0f, 0.0f};

					Vertex v1;
					v1.position = positions[b.positionIndex];
					v1.color = faceColor;
					if (b.textureIndex >= 0)
						v1.uv = texCoords[b.textureIndex];
					else
						v1.uv = {0.0f, 0.0f};

					Vertex v2;
					v2.position = positions[c.positionIndex];
					v2.color = faceColor;
					if (c.textureIndex >= 0)
						v2.uv = texCoords[c.textureIndex];
					else
						v2.uv = {0.0f, 0.0f};

					vertices.push_back(v0);
					vertices.push_back(v1);
					vertices.push_back(v2);

					indices.push_back(indexOffset + 0);
					indices.push_back(indexOffset + 1);
					indices.push_back(indexOffset + 2);
				}

				++faceIndex;
			}
		}

		return new Mesh(vertices, indices);
	}
}
