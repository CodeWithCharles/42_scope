#pragma once

#include "math/Vec3.hpp"

#include <string>

namespace Scop
{
	struct Material
	{
		std::string	name;

		Math::Vec3	ambient{ 0.0f, 0.0f, 0.0f };
		Math::Vec3	diffuse{ 1.0f, 1.0f, 1.0f };
		Math::Vec3	specular{ 0.0f, 0.0f, 0.0f };

		float		shininess = 0.0f;
		float		opacity = 1.0f;
		int			illuminationModel = 0;

		std::string	diffuseTexturePath;
	};
}
