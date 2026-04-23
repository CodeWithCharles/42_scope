#include "io/ObjLoader.hpp"
#include "io/MtlLoader.hpp"
#include "io/PathUtils.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
	struct ObjSection
	{
		std::string					name;
		std::string					materialName;
		std::vector<Scop::Vertex>	vertices;
		std::vector<unsigned int>	indices;
	};

	void	appendSectionToModel(Scop::Model& model, ObjSection& section)
	{
		if (section.vertices.empty())
			return;

		Scop::ModelPart	part;
		part.name = section.name;
		part.materialName = section.materialName;
		part.mesh = new Scop::Mesh(section.vertices, section.indices);

		model.addPart(part);

		section.vertices.clear();
		section.indices.clear();
	}
}

namespace Scop
{
	Model*	ObjLoader::load(const std::string& path)
	{
		std::ifstream file(path.c_str());
		if (!file.is_open())
			throw std::runtime_error("Failed to open OBJ file: " + path);

		Model* model = new Model();

		std::vector<Math::Vec3>	positions;
		std::vector<Math::Vec2>	texCoords;
		std::vector<Math::Vec3>	normals;

		ObjSection currentSection;
		currentSection.name = "Default";
		std::string objDirectory = getDirectoryPath(path);

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
			else if (prefix == "o")
			{
				std::string	previousMaterialName = currentSection.materialName;

				appendSectionToModel(*model, currentSection);

				currentSection = ObjSection();
				currentSection.materialName = previousMaterialName;
				lineStream >> currentSection.name;

				if (currentSection.name.empty())
					currentSection.name = "Unnamed";
			}
			else if (prefix == "mtllib")
			{
				std::string	mtlFileName;
				lineStream >> mtlFileName;

				if (!mtlFileName.empty())
				{
					std::string mtlPath = resolveResourcePath(objDirectory, mtlFileName);
					std::string mtlDirectory = getDirectoryPath(mtlPath);
					std::vector<Material> materials = MtlLoader::load(mtlPath);

					for (std::size_t i = 0; i < materials.size(); ++i)
					{
						if (!materials[i].diffuseTexturePath.empty())
						{
							materials[i].diffuseTexturePath = resolveResourcePath(
								mtlDirectory,
								materials[i].diffuseTexturePath);
						}
						model->addMaterial(materials[i]);
					}
				}
			}
			else if (prefix =="usemtl")
			{
				std::string materialName;
				lineStream >> materialName;

				appendSectionToModel(*model, currentSection);

				ObjSection newSection;
				newSection.name = currentSection.name;
				newSection.materialName = materialName;
				currentSection = newSection;
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

				const Scop::Material* material = model->findMaterialByName(
					currentSection.materialName);

				if (material != nullptr)
					faceColor = material->diffuse;

				for (std::size_t i = 1; i + 1 < faceVertices.size(); ++i)
				{
					const FaceVertex& a = faceVertices[0];
					const FaceVertex& b = faceVertices[i];
					const FaceVertex& c = faceVertices[i + 1];

					unsigned int indexOffset = static_cast<unsigned int>(currentSection.vertices.size());


					Vertex v0 = buildVertex(a, positions, texCoords, faceColor);
					Vertex v1 = buildVertex(b, positions, texCoords, faceColor);
					Vertex v2 = buildVertex(c, positions, texCoords, faceColor);

					currentSection.vertices.push_back(v0);
					currentSection.vertices.push_back(v1);
					currentSection.vertices.push_back(v2);

					currentSection.indices.push_back(indexOffset + 0);
					currentSection.indices.push_back(indexOffset + 1);
					currentSection.indices.push_back(indexOffset + 2);
				}

				++faceIndex;
			}
		}
		appendSectionToModel(*model, currentSection);
		return model;
	}

	std::vector<std::string>	ObjLoader::split(
		const std::string& value,
		char delimiter)
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


	int	ObjLoader::parseObjIndex(
		const std::string& token,
		std::size_t currentCount)
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

	ObjLoader::FaceVertex	ObjLoader::parseFaceVertexToken(
		const std::string& token,
		std::size_t positionsCount,
		std::size_t texCoordsCount,
		std::size_t normalsCount)
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

	Vertex	ObjLoader::buildVertex(
		const FaceVertex& faceVertex,
		const std::vector<Math::Vec3>& positions,
		const std::vector<Math::Vec2>& texCoords,
		const Math::Vec3& faceColor)
	{
		Vertex vertex;

		vertex.position = positions[faceVertex.positionIndex];
		vertex.color = faceColor;

		if (faceVertex.textureIndex >= 0)
		{
			vertex.uv = texCoords[faceVertex.textureIndex];
			vertex.uv.y = 1.0f - vertex.uv.y;
		}
		else
			vertex.uv = {0.0f, 0.0f};

		return vertex;
	}
}
