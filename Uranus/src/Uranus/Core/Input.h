#pragma once

#include "Uranus/Core/Core.h"
#include "Uranus/Core/KeyCodes.h"

namespace Uranus {

	class URANUS_API Input {
	public:
		static bool IsKeyPressed(int keyCode);
		static bool IsKeyPressed(KeyCode keyCode);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
