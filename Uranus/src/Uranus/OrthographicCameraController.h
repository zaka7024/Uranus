#pragma once
#include "Uranus/Renderer/OrthographicCamera.h"
#include "Uranus/Events/MouseEvent.h"
#include "Uranus/Events/ApplicationEvent.h"
#include "Uranus/Core/Timestep.h"

namespace Uranus {

	struct OrthographicCameraBounds {
		float Left, Right, Bottom, Top;

		uint32_t GetWidth() { return Left - Right; }
		uint32_t GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		const OrthographicCameraBounds& GetBounds() const { return _Bounds; }

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		OrthographicCamera& GetCamera() { return _Camera; }
	private:
		float _AspectRatio;
		float _ZoomLevel = 2.0f;
		glm::vec3 _CameraPosition = {0.0f, 0.0f, 0.0f};
		float _CameraRotaion = 0.0f;
		float _CameraMoveSpeed = 10.0f;
		float _CameraRotaionSpeed = 180.0f;
		bool _Rotation;
		OrthographicCameraBounds _Bounds;
		OrthographicCamera _Camera;
	};

}