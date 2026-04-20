#pragma once

#include <cstdint>

namespace Scope::Config
{
	inline constexpr char	WINDOW_TITLE[] = "SCOPE";

	inline constexpr int	WINDOW_WIDTH = 1280;
	inline constexpr int	WINDOW_HEIGHT = 720;

	inline constexpr int	OPENGL_VERSION_MAJOR = 4;
	inline constexpr int	OPENGL_VERSION_MINOR = 1;

	inline constexpr float	CLEAR_RED = 0.08f;
	inline constexpr float	CLEAR_GREEN = 0.09f;
	inline constexpr float	CLEAR_BLUE = 0.12f;
	inline constexpr float	CLEAR_ALPHA = 1.0f;

	inline constexpr bool	START_WITH_VSYNC = true;
}
