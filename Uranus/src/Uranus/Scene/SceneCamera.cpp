#include "urpch.h"

#include "SceneCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Uranus {

	SceneCamera::SceneCamera() {
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		_OrthographicSize = size;
		_OrthographicNear = nearClip;
		_OrthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		_AspectRatio = width / (float)height;
		float left = -_OrthographicSize * 0.5f * _AspectRatio;
		float right = _OrthographicSize * 0.5f * _AspectRatio;
		float bottom = -_OrthographicSize * 0.5f;
		float top = _OrthographicSize * 0.5f;

		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		float left = -_OrthographicSize * 0.5f * _AspectRatio;
		float right = _OrthographicSize * 0.5f * _AspectRatio;
		float bottom = -_OrthographicSize * 0.5f;
		float top = _OrthographicSize * 0.5f;

		_Projection = glm::ortho(left, right,
			bottom, top, _OrthographicNear, _OrthographicFar);
	}
}
