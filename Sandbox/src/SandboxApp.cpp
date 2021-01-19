
#include <Uranus.h>

class ExampleLayer : public Uranus::Layer {

public:
	ExampleLayer()
		:Layer("ExampleLayer") {}

	void OnUpdate() {
		std::stringstream ss;
		ss << GetLayerName() << "::OnUpdate";
		UR_INFO(ss.str())
	}

	void OnEvent(Uranus::Event& event) {
		UR_TRACE("{0}", event.ToString())
	}

};

class Sandbox : public Uranus::Application {

public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Uranus::ImGuiLayer());
	}

	~Sandbox() {

	}
};

Uranus::Application* CreateApplication() {
	return new Sandbox();
}