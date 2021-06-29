#pragma once

#include "Uranus/Core/Core.h"
#include "Uranus/Core/Layer.h"
#include "Uranus/Events/MouseEvent.h"
#include "Uranus/Events/KeyEvent.h"
#include "Uranus/Events/ApplicationEvent.h"

namespace Uranus {

	class URANUS_API ImGuiLayer : public Layer {

	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();

	private:
		float _time = 0.0f;
	};
}
