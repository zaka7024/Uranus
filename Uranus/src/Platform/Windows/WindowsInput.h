#pragma once

#include "Uranus/Core/Input.h"

namespace Uranus {

	class URANUS_API WindowsInput : public Input {
		virtual bool IsKeyPressedImpl(int keyCode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}
