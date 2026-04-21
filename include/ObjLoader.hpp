#pragma once

#include "Mesh.hpp"

#include <string>

namespace Scope
{
	class ObjLoader
	{
		public:
			static Mesh*	load(const std::string& path);
	};
}
