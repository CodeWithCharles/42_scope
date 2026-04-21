#pragma once

#include "Mesh.hpp"

#include <string>

namespace Scop
{
	class ObjLoader
	{
		public:
			static Mesh*	load(const std::string& path);
	};
}
