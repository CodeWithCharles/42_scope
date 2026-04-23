#pragma once

#include "scene/Model.hpp"
#include "render/Vertex.hpp"
#include "math/Vec2.hpp"
#include "math/Vec3.hpp"

#include <string>
#include <vector>

namespace Scop
{
	class ObjLoader
	{
		private:
			struct FaceVertex
			{
				int positionIndex;
				int textureIndex;
				int normalIndex;
			};

			static std::vector<std::string> split(
				const std::string& value,
				char delimiter);

			static int parseObjIndex(
				const std::string& token,
				std::size_t currentCount);

			static FaceVertex parseFaceVertexToken(
				const std::string& token,
				std::size_t positionsCount,
				std::size_t texCoordsCount,
				std::size_t normalsCount);

			static Vertex buildVertex(
				const FaceVertex& faceVertex,
				const std::vector<Math::Vec3>& positions,
				const std::vector<Math::Vec2>& texCoords,
				const Math::Vec3& faceColor);

		public:
			static Model*	load(const std::string& path);
	};
}
