#pragma once

namespace Scop
{
	enum class TextureSourceMode
	{
		PolygonColor,
		FallbackTexture,
		MaterialTexture
	};

	enum class PolygonMode
	{
		Filled,
		Wireframe,
		Point
	};

	enum class ShadingColorMode
	{
		Color,
		Grey
	};

	enum class ShadingInterpolationMode
	{
		Flat,
		Smooth
	};
}
