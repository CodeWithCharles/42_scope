#pragma once

#include "app/AppOptions.hpp"

#include <string>

namespace Scop
{
	class CliParser
	{
		public:
			static AppOptions	parse(int argc, char** argv);
			static std::string	buildUsage(const std::string& programName);

		private:
			static bool			hasObjExtension(const std::string& path);
			static bool			hasPpmExtension(const std::string& path);
	};
}
