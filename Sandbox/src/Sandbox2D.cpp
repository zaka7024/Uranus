#include "Sandbox2D.h"

#include <Uranus.h>

#include <imgui\imgui.cpp>
#include <glm\glm\gtc\type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), _CameraController(1280.0f / 720.0f, true)
{

}

void Sandbox2D::OnAttach()
{
	
	_SquareVA = Uranus::VertexArray::Create();
	_SquareVA->Bind();

	float verticesBlue[4 * 3] = {
		-0.5f, -0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f,
	};

	Uranus::Ref<Uranus::VertexBuffer> vertexBufferBlue;
	vertexBufferBlue = Uranus::VertexBuffer::Create(verticesBlue, sizeof(verticesBlue));

	Uranus::BufferLayout bufferLayoutBlue = {
		{ Uranus::ShaderDataType::Float3, "a_Position"},
	};

	vertexBufferBlue->SetLayout(bufferLayoutBlue);
	_SquareVA->AddVertexBuffer(vertexBufferBlue);


	uint32_t indeicesBlue[6] = { 0, 1, 2, 1, 3, 2 };
	Uranus::Ref<Uranus::IndexBuffer> indexBufferBlue;
	indexBufferBlue = (Uranus::IndexBuffer::Create(indeicesBlue, sizeof(indeicesBlue) / sizeof(uint32_t)));

	_SquareVA->SetIndexBuffer(indexBufferBlue);

	_FlatColorShader = Uranus::Shader::Create("assets/shaders/FlatColor.glsl");
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
	Uranus::Renderer2D::DrawQuad(_Position, { 1.0f, 1.0f }, _Roation, _Color);
	Uranus::Renderer2D::DrawQuad({ -0.42f, 0.0f }, { 1.0f, 3.0f }, 0, { 0.3f, 0.2f, 0.7f, 1 });
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
	ImGui::SliderFloat2("position", glm::value_ptr(_Position),-10, 10);
	ImGui::DragFloat("Rotation", &_Roation, 1, 0, 360);
	ImGui::End();
}