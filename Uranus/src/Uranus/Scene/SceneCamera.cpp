#include "urpch.h"

#include "SceneCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Uranus {

	SceneCamera::SceneCamera() {
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		_ProjectionType = ProjectionType::Orthographic;

		_OrthographicSize = size;
		_OrthographicNear = nearClip;
		_OrthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		_ProjectionType = ProjectionType::Perspective;
		_PerspectiveFOV = verticalFOV;
		_PerspectiveNear = nearClip;
		_PerspectiveFar = farClip;
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		_AspectRatio = (float)width / (float)height;
		float left = -_OrthographicSize * 0.5f * _AspectRatio;
		float right = _OrthographicSize * 0.5f * _AspectRatio;
		float bottom = -_OrthographicSize * 0.5f;
		float top = _OrthographicSize * 0.5f;

		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (_ProjectionType == ProjectionType::Orthographic) {
			float left = -_OrthographicSize * 0.5f * _AspectRatio;
			float right = _OrthographicSize * 0.5f * _AspectRatio;
			float bottom = -_OrthographicSize * 0.5f;
			float top = _OrthographicSize * 0.5f;

			_Projection = glm::ortho(left, right,
				bottom, top, _OrthographicNear, _OrthographicFar);
		}
		else {
			_Projection = glm::perspective(_PerspectiveFOV, _AspectRatio, _PerspectiveNear, _PerspectiveFar);
		}
	}
}
