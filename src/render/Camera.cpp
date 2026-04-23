#include "render/Camera.hpp"

namespace Scop
{
	Camera::Camera(
		float distance,
		float fovRadians,
		float nearPlane,
		float farPlane)
		:	m_distance(distance),
			m_fovRadians(fovRadians),
			m_nearPlane(nearPlane),
			m_farPlane(farPlane)
		{
		}

	Math::Mat4	Camera::getViewMatrix() const
	{
		return Math::Mat4::translation({0.0f, 0.0f, -m_distance});
	}

	Math::Mat4	Camera::getProjectionMatrix(
		int viewportWidth,
		int viewportHeight) const
	{
		float aspectRatio = static_cast<float>(viewportWidth)
			/ static_cast<float>(viewportHeight);

		return Math::Mat4::perspective(
			m_fovRadians,
			aspectRatio,
			m_nearPlane,
			m_farPlane);
	}
}
