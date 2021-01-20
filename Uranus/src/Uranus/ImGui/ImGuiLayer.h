#pragma once

#include "Uranus/Core.h"
#include "Uranus/Layer.h"
#include "Uranus/Events/MouseEvent.h"
#include "Uranus/Events/KeyEvent.h"
#include "Uranus/Events/ApplicationEvent.h"

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
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonRelesedEvent(MouseButtonRelesedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		float _time = 0.0f;
	};
}