#pragma once

#include "Uranus/Core/Timestep.h"
#include "Uranus/Events/MouseEvent.h"
#include "Camera.h"

namespace Uranus {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline float GetDistance() const { return _Distance; }
		inline void SetDistance(float distance) { _Distance = distance; }

		inline void SetViewportSize(float width, float height) { _ViewportWidth = width; _ViewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return _ViewMatrix; }
		glm::mat4 GetViewProjection() const { return _Projection * _ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return _Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return _Pitch; }
		float GetYaw() const { return _Yaw; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float _FOV = 45.0f, _AspectRatio = 1.778f, _NearClip = 0.1f, _FarClip = 1000.0f;

		glm::mat4 _ViewMatrix;
		glm::vec3 _Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 _FocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 _InitialMousePosition = { 0.0f, 0.0f };

		float _Distance = 10.0f;
		float _Pitch = 0.0f, _Yaw = 0.0f;

		float _ViewportWidth = 1280, _ViewportHeight = 720;
	};
}
