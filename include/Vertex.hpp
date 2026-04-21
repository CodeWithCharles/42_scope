#pragma once

#include <cstddef>

namespace Scope
{
	struct	Vec2f
	{
		float	x;
		float	y;
	};

	struct	Vec3f
	{
		float	x;
		float	y;
		float	z;
	};

	struct	Vertex
	{
		Vec3f	position;
		Vec3f	color;
		Vec2f	uv;

		static constexpr	unsigned int	POSITION_LOCATION = 0;
		static constexpr	unsigned int	COLOR_LOCATION = 1;
		static constexpr	unsigned int	UV_LOCATION = 2;

		static constexpr	int	POSITION_COMPONENT_COUNT = 3;
		static constexpr	int	COLOR_COMPONENT_COUNT = 3;
		static constexpr	int	UV_COMPONENT_COUNT = 2;
	};
}
