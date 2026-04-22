#pragma once

#include "Model.hpp"

#include <string>

namespace Scop
{
	class ObjLoader
	{
		public:
			static Model*	load(const std::string& path);
	};
}
