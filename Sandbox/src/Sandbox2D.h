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
	Uranus::Ref<Uranus::FrameBuffer> _FrameBuffer;
	
	Uranus::Ref<Uranus::Texture2D> _CheckerboardTexture;
	Uranus::Ref<Uranus::Texture2D> _PlayerTexture;
	Uranus::Ref<Uranus::Texture2D> _TileTexture;

	glm::vec4 _Color = {0.4f, 0.2f, 0.7f, 1.0f };
	glm::vec3 _Position = { 0.0f, 0.0f, 0.1f };
	glm::vec2 _Scale = { 1.0f, 1.0f };
	float _Rotation = { 0.0f };
};