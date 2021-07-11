#include "urpch.h"
#include "OrthographicCameraController.h"
#include "Uranus/Core/Input.h"
#include "Uranus/Core/KeyCodes.h"

namespace Uranus{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:_AspectRatio(aspectRatio), _Bounds ({ -_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel,
			-_ZoomLevel, _ZoomLevel }), _Camera(-_AspectRatio * _ZoomLevel, _AspectRatio* _ZoomLevel,
			-_ZoomLevel, _ZoomLevel), _Rotation(rotation)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

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

	void OrthographicCameraController::OnResize(uint32_t width, uint32_t height)
	{
		_AspectRatio = static_cast<float>(width) / height;
		_Bounds = { -_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel };
		_Camera.SetProjection(_Bounds.Left, _Bounds.Right, _Bounds.Bottom, _Bounds.Top);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		_ZoomLevel -= e.GetOffsetY() * 0.15f;
		_ZoomLevel = std::max(_ZoomLevel, 0.25f);
		_Bounds = { -_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel };
		_Camera.SetProjection(_Bounds.Left, _Bounds.Right, _Bounds.Bottom, _Bounds.Top);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		//_AspectRatio = static_cast<float>(e.GetWidth()) / e.GetHeight();
		//_Bounds = { -_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel };
		//_Camera.SetProjection(_Bounds.Left, _Bounds.Right, _Bounds.Bottom, _Bounds.Top);
		return false;
	}
}
