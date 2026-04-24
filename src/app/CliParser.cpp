#include "app/CliParser.hpp"

#include <stdexcept>

namespace
{
	bool	hasExtension(const std::string& path, const std::string& extension)
	{
		if (path.size() < extension.size())
			return false;

		return path.compare(path.size() - extension.size(), extension.size(), extension) == 0;
	}
}

namespace Scop
{
	AppOptions	CliParser::parse(int argc, char** argv)
	{
		if (argc < 2 || argc > 3)
			throw std::runtime_error(buildUsage(argc > 0 ? argv[0] : "./scop"));

		AppOptions	options;
		options.modelPath = argv[1];

		if (!hasObjExtension(options.modelPath))
			throw std::runtime_error("Model file must use the .obj extension\n"
				+ buildUsage(argv[0]));

		if (argc == 3)
		{
			options.fallbackTexturePath = argv[2];

			if (!hasPpmExtension(options.fallbackTexturePath))
				throw std::runtime_error("Fallback texture must use the .ppm extension\n"
					+ buildUsage(argv[0]));
		}

		return options;
	}

	std::string	CliParser::buildUsage(const std::string& programName)
	{
		return "Usage: " + programName + " <model.obj> [fallback_texture.ppm]";
	}

	bool	CliParser::hasObjExtension(const std::string& path)
	{
		return hasExtension(path, ".obj");
	}

	bool	CliParser::hasPpmExtension(const std::string& path)
	{
		return hasExtension(path, ".ppm");
	}
}
