#pragma once

#include "math/Mat4.hpp"
#include "math/Vec3.hpp"
#include "scene/Model.hpp"
#include "render/Camera.hpp"

namespace Scop
{
	class ModelTransformBuilder
	{
		public:
			static Math::Mat4	buildModelMatrix(
				const Model& model,
				const Math::Vec3& position,
				float rotationAngle,
				float importRotationXRadians);

			static Math::Mat4	buildMvpMatrix(
				const Model& model,
				const Camera& camera,
				const Math::Vec3& position,
				float rotationAngle,
				float importRotationXRadians,
				int viewportWidth,
				int viewportHeight);
	};
}
