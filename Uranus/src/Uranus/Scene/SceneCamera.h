#pragma once

#include "Uranus/Renderer/Camera.h"

namespace Uranus {

	class SceneCamera : public Camera {
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() const { return _OrthographicSize; }
		void SetOrthographicSize(float size) { _OrthographicSize = size; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		float _OrthographicSize = 10.0f, _OrthographicNear = -1.0f, _OrthographicFar = 1.0f;
		float m_AspectRatio = 0.0f;
	};
}