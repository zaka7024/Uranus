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
		Ref<Shader> TextureShader;
	};

	Renderer2DState* _RendererData;

	void Renderer2D::Init()
	{
		_RendererData = new Renderer2DState();

		_RendererData->QuadVertexArray = VertexArray::Create();
		_RendererData->QuadVertexArray->Bind();

		float vertices[4 * 5] = {
			-0.5f, -0.5f,  0.0f, 0.0, 0.0,
			 0.5f, -0.5f,  0.0f, 1.0, 0.0,
			-0.5f,  0.5f,  0.0f, 0.0, 1.0,
			 0.5f,  0.5f,  0.0f, 1.0, 1.0
		};

		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		BufferLayout bufferLayout = {
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float2, "a_TexCoord"},
		};

		vertexBuffer->SetLayout(bufferLayout);
		_RendererData->QuadVertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indeicesBlue[6] = { 0, 1, 2, 1, 3, 2 };
		Ref<IndexBuffer> indexBufferBlue;
		indexBufferBlue = (IndexBuffer::Create(indeicesBlue, sizeof(indeicesBlue) / sizeof(uint32_t)));

		_RendererData->QuadVertexArray->SetIndexBuffer(indexBufferBlue);

		_RendererData->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
		_RendererData->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete _RendererData;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		_RendererData->FlatColorShader->Bind();
		_RendererData->FlatColorShader->SetMat4(camera.GetViewProjectionMatrix(), "u_ViewProjection");

		_RendererData->TextureShader->Bind();
		_RendererData->TextureShader->SetMat4(camera.GetViewProjectionMatrix(), "u_ViewProjection");
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		_RendererData->FlatColorShader->Bind();
		_RendererData->FlatColorShader->SetFloat4(color, "u_Color");

		auto transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		_RendererData->FlatColorShader->SetMat4(transform, "u_Transform");

		_RendererData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_RendererData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture)
	{
		_RendererData->TextureShader->Bind();
		texture->Bind();
		
		auto transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		_RendererData->TextureShader->SetMat4(transform, "u_Transform");

		_RendererData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_RendererData->QuadVertexArray);
	}
}
