#include "urpch.h"
#include "OrthographicCameraController.h"
#include "Uranus/Core/Input.h"
#include "Uranus/Core/KeyCodes.h"

namespace Uranus{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:_AspectRatio(aspectRatio), _Camera(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel,
			-_ZoomLevel, _ZoomLevel), _Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (Uranus::Input::IsKeyPressed(UR_KEY_W))
			_CameraPosition.y += _CameraMoveSpeed * ts;
		if (Uranus::Input::IsKeyPressed(UR_KEY_S))
			_CameraPosition.y -= _CameraMoveSpeed * ts;

		if (Uranus::Input::IsKeyPressed(UR_KEY_D))
			_CameraPosition.x += _CameraMoveSpeed * ts;
		if (Uranus::Input::IsKeyPressed(UR_KEY_A))
			_CameraPosition.x -= _CameraMoveSpeed * ts;

		if (_Rotation) {
			if (Uranus::Input::IsKeyPressed(UR_KEY_Q))
				_CameraRotaion += _CameraRotaionSpeed * ts;
			if (Uranus::Input::IsKeyPressed(UR_KEY_E))
				_CameraRotaion -= _CameraRotaionSpeed * ts;

			_Camera.SetRotation(_CameraRotaion);
		}

		_Camera.SetPoisition(_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(UR_BIND_EVENT_FUN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(UR_BIND_EVENT_FUN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		_ZoomLevel -= e.GetOffsetY() * 0.15f;
		_ZoomLevel = std::max(_ZoomLevel, 0.25f);
		_Camera.SetProjection(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		_AspectRatio = static_cast<float>(e.GetWidth()) / e.GetHeight();
		_Camera.SetProjection(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel);
		return false;
	}
}
