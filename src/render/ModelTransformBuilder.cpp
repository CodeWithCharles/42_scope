#include "render/ModelTransformBuilder.hpp"

namespace Scop
{
	Math::Mat4	ModelTransformBuilder::buildModelMatrix(
		const Model& model,
		const Math::Vec3& position,
		float rotationAngle,
		float importRotationXRadians)
	{
		Math::Vec3 center = model.getCenter();
		Math::Vec3 size = model.getSize();

		float maxExtent = size.x;

		if (size.y > maxExtent)
			maxExtent = size.y;
		if (size.z > maxExtent)
			maxExtent = size.z;

		float scaleFactor = 1.0f;
		if (maxExtent > 0.0f)
			scaleFactor = 1.0f / maxExtent;

		Math::Mat4 toOrigin = Math::Mat4::translation({
			-center.x,
			-center.y,
			-center.z
		});

		Math::Mat4 scale = Math::Mat4::scale({
			scaleFactor,
			scaleFactor,
			scaleFactor
		});

		Math::Mat4 importRotation = Math::Mat4::rotationX(importRotationXRadians);
		Math::Mat4 animationRotation = Math::Mat4::rotationY(rotationAngle);
		Math::Mat4 rotation = animationRotation * importRotation;
		Math::Mat4 translation = Math::Mat4::translation(position);

		return translation * rotation * scale * toOrigin;
	}

	Math::Mat4	ModelTransformBuilder::buildMvpMatrix(
		const Model& model,
		const Camera& camera,
		const Math::Vec3& position,
		float rotationAngle,
		float importRotationXRadians,
		int viewportWidth,
		int viewportHeight)
	{
		Math::Mat4	modelMatrix = buildModelMatrix(
			model,
			position,
			rotationAngle,
			importRotationXRadians);

		Math::Mat4 view = camera.getViewMatrix();
		Math::Mat4 projection = camera.getProjectionMatrix(
			viewportWidth,
			viewportHeight);

		return projection * view * modelMatrix;
	}
}
