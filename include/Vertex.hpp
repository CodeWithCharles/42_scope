#pragma once

#include "math/Vec2.hpp"
#include "math/Vec3.hpp"

#include <cstddef>

namespace Scope
{
	struct	Vertex
	{
		Math::Vec3	position;
		Math::Vec3	color;
		Math::Vec2	uv;

		static constexpr	unsigned int	POSITION_LOCATION = 0;
		static constexpr	unsigned int	COLOR_LOCATION = 1;
		static constexpr	unsigned int	UV_LOCATION = 2;

		static constexpr	int	POSITION_COMPONENT_COUNT = 3;
		static constexpr	int	COLOR_COMPONENT_COUNT = 3;
		static constexpr	int	UV_COMPONENT_COUNT = 2;
	};
}
