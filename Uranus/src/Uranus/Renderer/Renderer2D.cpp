#include "urpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Uranus {

	static struct Renderer2DState {
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	Renderer2DState* _RendererData;

	void Renderer2D::Init()
	{
		_RendererData = new Renderer2DState();

		_RendererData->QuadVertexArray = VertexArray::Create();
		_RendererData->QuadVertexArray->Bind();

		float verticesBlue[4 * 3] = {
			-0.2f, -0.2f,  0.6f,
			 0.2f, -0.2f,  0.6f,
			-0.2f,  0.2f,  0.6f,
			 0.2f,  0.2f,  0.6f,
		};

		Ref<VertexBuffer> vertexBufferBlue;
		vertexBufferBlue = VertexBuffer::Create(verticesBlue, sizeof(verticesBlue));

		BufferLayout bufferLayoutBlue = {
			{ ShaderDataType::Float3, "a_Position"},
		};

		vertexBufferBlue->SetLayout(bufferLayoutBlue);
		_RendererData->QuadVertexArray->AddVertexBuffer(vertexBufferBlue);


		uint32_t indeicesBlue[6] = { 0, 1, 2, 1, 3, 2 };
		Ref<IndexBuffer> indexBufferBlue;
		indexBufferBlue = (IndexBuffer::Create(indeicesBlue, sizeof(indeicesBlue) / sizeof(uint32_t)));

		_RendererData->QuadVertexArray->SetIndexBuffer(indexBufferBlue);

		_RendererData->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete _RendererData;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		_RendererData->FlatColorShader->Bind();
		_RendererData->FlatColorShader->SetMat4(camera.GetViewProjectionMatrix(), "u_ViewProjection");
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float roation, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, roation, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float roation, const glm::vec4& color)
	{
		_RendererData->FlatColorShader->Bind();
		_RendererData->FlatColorShader->SetFloat4(color, "u_Color");

		auto transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(roation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		_RendererData->FlatColorShader->SetMat4(transform, "u_Transform");

		_RendererData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_RendererData->QuadVertexArray);
	}
}
