#pragma once

#include "math/Vec3.hpp"
#include "render/RenderState.hpp"

struct GLFWwindow;

namespace Scop
{
	class InputController
	{
		private:
			bool	m_textureModeTogglePressed;
			bool	m_polygonModeTogglePressed;
		public:
			InputController();

			void	update(
				GLFWwindow* window,
				float deltaTime,
				Math::Vec3& position,
				RenderState& renderState);

		private:
			void	handleMovement(
				GLFWwindow* window,
				float deltaTime,
				Math::Vec3& position);

			void	handleTextureMode(
				GLFWwindow* window,
				RenderState& renderState);

			void	handlePolygonMode(
				GLFWwindow* window,
				RenderState& renderState);

			void	cycleTextureSourceMode(RenderState& renderState);
			void	cyclePolygonMode(RenderState& renderState);
	};
}
