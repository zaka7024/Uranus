#pragma once

#include "Uranus/Core/Core.h"

namespace Uranus {

	class URANUS_API Input {
	public:
		static bool IsKeyPressed(int keyCode);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
