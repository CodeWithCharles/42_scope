#include "app/InputController.hpp"

#include "core/Config.hpp"

#include <GLFW/glfw3.h>

namespace Scop
{
	InputController::InputController()
		: m_textureModeTogglePressed(false),
		  m_polygonModeTogglePressed(false)
	{
	}

	void	InputController::update(
		GLFWwindow* window,
		float deltaTime,
		Math::Vec3& position,
		RenderState& renderState)
	{
		handleMovement(window, deltaTime, position);
		handleTextureMode(window, renderState);
		handlePolygonMode(window, renderState);
	}

	void	InputController::handleMovement(
		GLFWwindow* window,
		float deltaTime,
		Math::Vec3& position)
	{
		float	translationStep = Config::TRANSLATION_SPEED_UNITS_PER_SECOND * deltaTime;

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			position.x += translationStep;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			position.x -= translationStep;

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			position.y += translationStep;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			position.y -= translationStep;

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			position.z -= translationStep;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			position.z += translationStep;
	}

	void	InputController::handleTextureMode(
		GLFWwindow* window,
		RenderState& renderState)
	{
		int	textureToggleState = glfwGetKey(window, GLFW_KEY_T);

		if (textureToggleState == GLFW_PRESS && !m_textureModeTogglePressed)
		{
			cycleTextureSourceMode(renderState);
			m_textureModeTogglePressed = true;
		}
		else if (textureToggleState == GLFW_RELEASE)
		{
			m_textureModeTogglePressed = false;
		}
	}

	void	InputController::handlePolygonMode(
		GLFWwindow* window,
		RenderState& renderState)
	{
		int	polygonToggleState = glfwGetKey(window, GLFW_KEY_R);

		if (polygonToggleState == GLFW_PRESS && !m_polygonModeTogglePressed)
		{
			cyclePolygonMode(renderState);
			m_polygonModeTogglePressed = true;
		}
		else if (polygonToggleState == GLFW_RELEASE)
		{
			m_polygonModeTogglePressed = false;
		}
	}

	void	InputController::cycleTextureSourceMode(RenderState& renderState)
	{
		if (renderState.textureSourceMode == TextureSourceMode::PolygonColor)
			renderState.textureSourceMode = TextureSourceMode::FallbackTexture;
		else if (renderState.textureSourceMode == TextureSourceMode::FallbackTexture)
			renderState.textureSourceMode = TextureSourceMode::MaterialTexture;
		else
			renderState.textureSourceMode = TextureSourceMode::PolygonColor;
	}

	void	InputController::cyclePolygonMode(RenderState& renderState)
	{
		if (renderState.polygonMode == PolygonMode::Filled)
			renderState.polygonMode = PolygonMode::Wireframe;
		else if (renderState.polygonMode == PolygonMode::Wireframe)
			renderState.polygonMode = PolygonMode::Point;
		else
			renderState.polygonMode = PolygonMode::Filled;
	}
}
