#pragma once

#include "Uranus/Core/Core.h"

namespace Uranus {

	class URANUS_API Input {
	public:
		static bool IsKeyPressed(int keyCode) { return (*_Instance).IsKeyPressedImpl(keyCode); };
		static bool IsMouseButtonPressed(int button) { return (*_Instance).IsMouseButtonPressedImpl(button); }
		static std::pair<float, float> GetMousePosition() { return (*_Instance).GetMousePositionImpl(); }
		static float GetMouseX() { return (*_Instance).GetMouseXImpl(); }
		static float GetMouseY() { return (*_Instance).GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keyCode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Scope<Input> _Instance;
	};
}