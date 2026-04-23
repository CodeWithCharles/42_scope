#include "io/PathUtils.hpp"

namespace Scop
{
	std::string	getDirectoryPath(const std::string& path)
	{
		std::size_t separatorPos = path.find_last_of("/\\");
		if (separatorPos == std::string::npos)
			return "";

		return path.substr(0, separatorPos + 1);
	}

	std::string	normalizePathSeparators(const std::string& path)
	{
		std::string normalized = path;

		for (std::size_t i = 0; i < normalized.size(); ++i)
		{
			if (normalized[i] == '\\')
				normalized[i] = '/';
		}
		return normalized;
	}

	bool	isAbsolutePath(const std::string& path)
	{
		if (path.empty())
			return false;
		if (path[0] == '/' || path[0] == '\\')
			return true;
		if (path.size() > 1 && path[1] == ':')
			return true;
		return false;
	}

	std::string	resolveResourcePath(
		const std::string& baseDirectory,
		const std::string& resourcePath)
	{
		if (resourcePath.empty())
			return "";

		std::string normalizedPath = normalizePathSeparators(resourcePath);

		if (isAbsolutePath(normalizedPath))
			return normalizedPath;
		return normalizePathSeparators(baseDirectory) + normalizedPath;
	}
}
