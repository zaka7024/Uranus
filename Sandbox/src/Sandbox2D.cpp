#include "Sandbox2D.h"

#include <Uranus.h>

#include <imgui\imgui.cpp>
#include <glm\glm\gtc\type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), _CameraController(1280.0f / 720.0f, true)
{

}

void Sandbox2D::OnAttach()
{
	_CheckerboardTexture = Uranus::Texture2D::Create("assets/textures/Checkerboard.png");
	_PlayerTexture = Uranus::Texture2D::Create("assets/textures/moon.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Uranus::Timestep ts)
{
	_CameraController.OnUpdate(ts);

	Uranus::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Uranus::RenderCommand::Clear();
	
	Uranus::Renderer2D::BeginScene(_CameraController.GetCamera());
	Uranus::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, 0.0f), { 10.0f, 10.0f }, 0, _CheckerboardTexture);
	Uranus::Renderer2D::DrawQuad(_Position, _Scale, _Roation, _Color);
	Uranus::Renderer::EndScene();
}

void Sandbox2D::OnEvent(Uranus::Event& event)
{
	_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Color Picker");
	ImGui::ColorEdit4("Color", glm::value_ptr(_Color));
	ImGui::SliderFloat3("Position", glm::value_ptr(_Position),-10, 10);
	ImGui::DragFloat("Rotation", &_Roation, 0.1, 0, 360);
	ImGui::DragFloat2("Scale", glm::value_ptr(_Scale), 0.1, 0, 360);
	ImGui::End();
}
