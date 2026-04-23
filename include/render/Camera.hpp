#pragma once

#include "math/Mat4.hpp"

namespace Scop
{
	class Camera
	{
		private:
			float	m_distance;
			float	m_fovRadians;
			float	m_nearPlane;
			float	m_farPlane;

		public:
			Camera(
				float distance,
				float fovRadians,
				float nearPlane,
				float farPlane);

			Math::Mat4	getViewMatrix() const;
			Math::Mat4	getProjectionMatrix(
				int viewportWidth,
				int viewportHeight) const;
	};
}
