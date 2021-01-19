#pragma once

#include "Uranus/Core.h"
#include "Uranus/Layer.h"

namespace Uranus {

	class URANUS_API ImGuiLayer : public Layer {

	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);

	private:
		float _time = 0.0f;
	};
}