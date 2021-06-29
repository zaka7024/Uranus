#include "urpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Uranus {

	static struct Renderer2DStorage {
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	Renderer2DStorage* _RendererData;

	void Renderer2D::Init()
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		_RendererData = new Renderer2DStorage();

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

		_RendererData->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		_RendererData->TextureShader->Bind();
		_RendererData->TextureShader->SetInt(0, "u_Texture");

		_RendererData->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		_RendererData->WhiteTexture->SetData(&data, sizeof(data));
	}

	void Renderer2D::Shutdown()
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		delete _RendererData;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		_RendererData->TextureShader->Bind();
		_RendererData->TextureShader->SetMat4(camera.GetViewProjectionMatrix(), "u_ViewProjection");
	}

	void Renderer2D::EndScene()
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		_RendererData->TextureShader->SetFloat4(color, "u_Color");
		_RendererData->TextureShader->SetFloat(1.0f, "u_TilingFactor");
		_RendererData->WhiteTexture->Bind();

		auto transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		_RendererData->TextureShader->SetMat4(transform, "u_Transform");

		_RendererData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_RendererData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		_RendererData->TextureShader->SetFloat4(glm::vec4(1.0f), "u_Color");
		_RendererData->TextureShader->SetFloat(tilingFactor, "u_TilingFactor");
		texture->Bind();
		
		auto transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		_RendererData->TextureShader->SetMat4(transform, "u_Transform");

		_RendererData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_RendererData->QuadVertexArray);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		_RendererData->TextureShader->SetFloat4(color, "u_Color");
		_RendererData->TextureShader->SetFloat(1.0f, "u_TilingFactor");
		_RendererData->WhiteTexture->Bind();

		auto transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0, 0.0, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		_RendererData->TextureShader->SetMat4(transform, "u_Transform");

		_RendererData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_RendererData->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		_RendererData->TextureShader->SetFloat4(glm::vec4(1.0f), "u_Color");
		_RendererData->TextureShader->SetFloat(tilingFactor, "u_TilingFactor");
		texture->Bind();

		auto transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0, 0.0, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		_RendererData->TextureShader->SetMat4(transform, "u_Transform");

		_RendererData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_RendererData->QuadVertexArray);
	}
}
