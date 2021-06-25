#pragma once
#include "Uranus.h"

class Sandbox2D : public Uranus::Layer {
public:
	Sandbox2D();
	~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Uranus::Timestep ts) override;
	virtual void OnEvent(Uranus::Event& event) override;
	virtual void OnImGuiRender() override;
private:
	Uranus::OrthographicCameraController _CameraController;
	Uranus::Ref<Uranus::VertexArray> _SquareVA;
	Uranus::Ref<Uranus::Shader> _FlatColorShader;
	glm::vec4 _Color = {0.4f, 0.5f, 0.1f, 1.0f };
};