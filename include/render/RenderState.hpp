#pragma once

#include "render/RenderModes.hpp"

namespace Scop
{
	struct AxisRotationState
	{
		bool	xEnabled = false;
		bool	yEnabled = true;
		bool	zEnabled = false;
	};

	struct RenderState
	{
		TextureSourceMode			textureSourceMode = TextureSourceMode::MaterialTexture;
		PolygonMode					polygonMode = PolygonMode::Filled;
		ShadingColorMode			shadingColorMode = ShadingColorMode::Color;
		ShadingInterpolationMode	shadingInterpolationMode = ShadingInterpolationMode::Smooth;

		float						opacity = 1.0f;
		float						textureBlend = 1.0f;

		AxisRotationState			rotation;
	};
}
