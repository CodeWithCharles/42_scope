#pragma once

#include "math/Vec3.hpp"

namespace Scope::Math
{
	struct Mat4
	{
		float	data[16];

		static Mat4	identity();
		static Mat4	translation(const Vec3& translation);
		static Mat4	scale(const Vec3& scale);
		static Mat4	rotationX(float angleRadians);
		static Mat4	rotationY(float angleRadians);
		static Mat4	rotationZ(float angleRadians);
		static Mat4	perspective(float fovRadians,
			float aspectRatio, float nearPlane, float farPlane);

		Mat4	operator*(const Mat4& other) const;
		const float*	raw() const;
		float*	raw();
	};
}
