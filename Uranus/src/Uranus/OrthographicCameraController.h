#pragma once
#include "Uranus/Renderer/OrthographicCamera.h"
#include "Uranus/Events/MouseEvent.h"
#include "Uranus/Events/ApplicationEvent.h"
#include "Uranus/Core/Timestep.h"

namespace Uranus {

	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		OrthographicCamera& GetCamera() { return _Camera; }
	private:
		float _AspectRatio;
		float _ZoomLevel = 1.0f;
		glm::vec3 _CameraPosition = {0.0f, 0.0f, 0.0f};
		float _CameraRotaion = 0.0f;
		float _CameraMoveSpeed = 1.0f;
		float _CameraRotaionSpeed = 180.0f;
		bool _Rotation;
		OrthographicCamera _Camera;

	};

}