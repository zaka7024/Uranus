
#include <Uranus.h>

#include "imgui/imgui.h"

class ExampleLayer : public Uranus::Layer {

public:
	ExampleLayer()
		:Layer("ExampleLayer") {}

	void OnUpdate() {

		if (Uranus::Input::IsKeyPressed(UR_KEY_A)) {
			UR_TRACE("UR_KEY_A Pressed (poll)")
		}
	}

	void OnEvent(Uranus::Event& event) {
		if (event.GetEventType() == Uranus::EventType::KeyPressed) {
			Uranus::KeyPressedEvent& e = (Uranus::KeyPressedEvent&)event;
			UR_TRACE((char)e.GetKeyCode())
		}
	}

	void OnImGuiRender() {
		ImGui::Begin("Window");
		ImGui::Text("Hello, World");
		ImGui::End();
	}
};

class Sandbox : public Uranus::Application {

public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		//PushOverlay(new Uranus::ImGuiLayer());
	}

	~Sandbox() {

	}
};

Uranus::Application* CreateApplication() {
	return new Sandbox();
}