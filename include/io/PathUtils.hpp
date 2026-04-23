#pragma once

#include <string>

namespace Scop
{
	std::string	getDirectoryPath(const std::string& path);
	std::string	normalizePathSeparators(const std::string& path);
	bool		isAbsolutePath(const std::string& path);
	std::string	resolveResourcePath(
		const std::string& baseDirectory,
		const std::string& resourcePath);
}
