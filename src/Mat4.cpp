#include "math/Mat4.hpp"
#include <cmath>

namespace Scope::Math
{
	Mat4	Mat4::identity()
	{
		Mat4 result = {};
		result.data[0] = 1.0f;
		result.data[5] = 1.0f;
		result.data[10] = 1.0f;
		result.data[15] = 1.0f;
		return result;
	}

	Mat4	Mat4::translation(const Vec3& translation)
	{
		Mat4 result = Mat4::identity();
		result.data[12] = translation.x;
		result.data[13] = translation.y;
		result.data[14] = translation.z;
		return result;
	}

	Mat4	Mat4::scale(const Vec3& scale)
	{
		Mat4 result = {};
		result.data[0] = scale.x;
		result.data[5] = scale.y;
		result.data[10] = scale.z;
		result.data[15] = 1.0f;
		return result;
	}

	Mat4	Mat4::operator*(const Mat4& other) const
	{
		Mat4 result = {};
		float sum = 0.0f;

		for (int col = 0; col < 4; ++col)
		{
			for (int row = 0; row < 4; ++row)
			{
				sum = 0.0f;

				for (int k = 0; k < 4; ++k)
				{
					sum += data[k * 4 + row] * other.data[col * 4 + k];
				}

				result.data[col * 4 + row] = sum;
			}
		}

		return result;
	}

	const float*	Mat4::raw() const
	{
		return data;
	}

	float*	Mat4::raw()
	{
		return data;
	}

	Mat4	Mat4::rotationX(float angleRadians)
	{
		Mat4 result = {};
		float cosAngle = std::cos(angleRadians);
		float sinAngle = std::sin(angleRadians);

		result.data[0] = 1.0f;
		result.data[5] = cosAngle;
		result.data[6] = sinAngle;
		result.data[9] = -sinAngle;
		result.data[10] = cosAngle;
		result.data[15] = 1.0f;

		return result;
	}

	Mat4	Mat4::rotationY(float angleRadians)
	{
		Mat4 result = {};
		float cosAngle = std::cos(angleRadians);
		float sinAngle = std::sin(angleRadians);

		result.data[0] = cosAngle;
		result.data[2] = -sinAngle;
		result.data[5] = 1.0f;
		result.data[8] = sinAngle;
		result.data[10] = cosAngle;
		result.data[15] = 1.0f;

		return result;
	}

	Mat4	Mat4::rotationZ(float angleRadians)
	{
		Mat4 result = {};
		float cosAngle = std::cos(angleRadians);
		float sinAngle = std::sin(angleRadians);

		result.data[0] = cosAngle;
		result.data[1] = sinAngle;
		result.data[4] = -sinAngle;
		result.data[5] = cosAngle;
		result.data[10] = 1.0f;
		result.data[15] = 1.0f;

		return result;
	}

	Mat4	Mat4::perspective(float fovRadians, float aspectRatio,
		float nearPlane, float farPlane)
	{
		Mat4 result = {};
		float f = 1.0f / std::tan(fovRadians / 2.0f);

		result.data[0] = f / aspectRatio;
		result.data[5] = f;
		result.data[10] = (farPlane + nearPlane) / (nearPlane - farPlane);
		result.data[11] = -1.0f;
		result.data[14] = (2.0f * farPlane * nearPlane) / (nearPlane - farPlane);

		return result;
	}
}
