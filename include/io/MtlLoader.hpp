#pragma once

#include "scene/Material.hpp"

#include <string>
#include <vector>

namespace Scop
{
	class MtlLoader
	{
		public:
			static std::vector<Material>	load(const std::string& path);
	};
}
