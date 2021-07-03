#include "Sandbox2D.h"

#include <Uranus.h>

#include <imgui\imgui.cpp>
#include <glm\glm\gtc\type_ptr.hpp>

#include <chrono>


Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), _CameraController(1280.0f / 720.0f, true)
{

}

void Sandbox2D::OnAttach()
{
	_CheckerboardTexture = Uranus::Texture2D::Create("assets/textures/Checkerboard.png");
	_PlayerTexture = Uranus::Texture2D::Create("assets/textures/moon.png");
	_TileTexture = Uranus::Texture2D::Create("assets/textures/uranus.jpg");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Uranus::Timestep ts)
{
	UR_PROFILE_FUNCTION();
	{
		UR_PROFILE_SCOPE("CameraController::OnUpdate");
		_CameraController.OnUpdate(ts);
	}

	{
		UR_PROFILE_SCOPE("Renderer Prep");
		Uranus::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Uranus::RenderCommand::Clear();
	}
	
	{
		UR_PROFILE_SCOPE("Renderer Draw");
		Uranus::Renderer2D::BeginScene(_CameraController.GetCamera());
		Uranus::Renderer2D::DrawQuad(glm::vec3(0.0f), { 10.0f, 10.0f }, _CheckerboardTexture, 10);
		
		static float angel;
		angel += ts * 80.0f;

		Uranus::Renderer2D::DrawRotatedQuad({-2.0f, 0.0f}, { 1.0f, 1.0f }, angel, _CheckerboardTexture, 10, { 1.0f, 0.8f, 0.8f, 1.0f });
		
		Uranus::Renderer2D::DrawQuad(_Position, { 1.0f, 1.0f }, _PlayerTexture, 1);
		
		for(uint32_t i = 0; i < 5; i++)
			Uranus::Renderer2D::DrawQuad({ 1.0f * i, 0.0f }, _Scale, _TileTexture);
		
		Uranus::Renderer2D::DrawRotatedQuad({ 1.0f, -1.0f }, _Scale, angel, {0.7, 0.2, 0.5, 1.0f});
		//Uranus::Renderer2D::DrawQuad(_Position - glm::vec3(_Scale.x / 2.0f, _Scale.y / 2.0f, 0), _Scale, _Color);
		//Uranus::Renderer2D::DrawRotatedQuad(_Position, _Scale, glm::radians(_Rotation), _Color);
	}
	
	Uranus::Renderer2D::EndScene();
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
	ImGui::DragFloat("Rotation", &_Rotation, 0.1, 0, 360);
	ImGui::DragFloat2("Scale", glm::value_ptr(_Scale), 0.1, 0, 360);

	for (auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();

	ImGui::End();
}