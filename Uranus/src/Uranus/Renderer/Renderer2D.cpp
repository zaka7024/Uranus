#include "urpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Uranus {

	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	static struct Renderer2DStorage {
		
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		// The wihte texture will bind to slot 0
		uint32_t TextureSlotIndex = 1;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;

		glm::vec4 QuadVertexPosition[4];
		glm::vec2 QuadTextureCoords[4];
	};

	Renderer2DStorage* _RendererData;

	void Renderer2D::Init()
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		_RendererData = new Renderer2DStorage();
		_RendererData->QuadVertexArray = VertexArray::Create();
		_RendererData->QuadVertexArray->Bind();
		
		_RendererData->QuadVertexBuffer = VertexBuffer::Create(_RendererData->MaxVertices * sizeof(QuadVertex));

		BufferLayout bufferLayout = {
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float4, "a_Color"},
			{ ShaderDataType::Float2, "a_TexCoord"},
			{ ShaderDataType::Float, "a_TexIndex"},
			{ ShaderDataType::Float, "a_TilingFactor"},
		};

		_RendererData->QuadVertexBuffer->SetLayout(bufferLayout);
		_RendererData->QuadVertexArray->AddVertexBuffer(_RendererData->QuadVertexBuffer);

		_RendererData->QuadVertexBufferBase = new QuadVertex[_RendererData->MaxVertices];

		uint32_t* quadIndices = new uint32_t[_RendererData->MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < _RendererData->MaxIndices; i += 6)
		{
			quadIndices[i + 0] = 0 + offset;
			quadIndices[i + 1] = 1 + offset;
			quadIndices[i + 2] = 2 + offset;
			quadIndices[i + 3] = 2 + offset;
			quadIndices[i + 4] = 3 + offset;
			quadIndices[i + 5] = 0 + offset;

			offset += 4;
		}

		Ref<IndexBuffer> quadIndexBuffer = (IndexBuffer::Create(quadIndices, _RendererData->MaxIndices));
		_RendererData->QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

		delete[] quadIndices;

		int32_t samplers[_RendererData->MaxTextureSlots];
		for (uint32_t i = 0; i < _RendererData->MaxTextureSlots; i++) {
			samplers[i] = i;
		}

		_RendererData->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		_RendererData->TextureShader->Bind();
		_RendererData->TextureShader->SetIntArray(samplers, _RendererData->MaxTextureSlots, "u_Textures");

		_RendererData->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		_RendererData->WhiteTexture->SetData(&data, sizeof(data));

		_RendererData->TextureSlots[0] = _RendererData->WhiteTexture;

		_RendererData->QuadVertexPosition[0] = {-0.5f, -0.5f, 0.0f, 0.0f };
		_RendererData->QuadVertexPosition[1] = { 0.5f, -0.5f, 0.0f, 0.0f };
		_RendererData->QuadVertexPosition[2] = { 0.5f,  0.5f, 0.0f, 0.0f };
		_RendererData->QuadVertexPosition[3] = {-0.5f,  0.5f, 0.0f, 0.0f };

		_RendererData->QuadTextureCoords[0] = { 0.0f,  0.0f };
		_RendererData->QuadTextureCoords[1] = { 1.0f,  0.0f };
		_RendererData->QuadTextureCoords[2] = { 1.0f,  1.0f };
		_RendererData->QuadTextureCoords[3] = { 0.0f,  1.0f };
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

		_RendererData->TextureSlotIndex = 1;

		_RendererData->QuadIndexCount = 0;
		_RendererData->QuadVertexBufferPtr = _RendererData->QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		uint32_t size = (uint8_t*)_RendererData->QuadVertexBufferPtr - (uint8_t*)_RendererData->QuadVertexBufferBase;
		_RendererData->QuadVertexBuffer->SetData(_RendererData->QuadVertexBufferBase, size);
	
		Flush();
	}

	void Renderer2D::Flush() {

		for (uint32_t i = 0; i < _RendererData->TextureSlotIndex; i++)
			_RendererData->TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(_RendererData->QuadVertexArray, _RendererData->QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		float texIndex = 0;
		float tilingFactor = 0;

		auto transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		for (uint32_t i = 0; i < 4; i++) {
			_RendererData->QuadVertexBufferPtr->Position = transform * _RendererData->QuadVertexPosition[i];
			_RendererData->QuadVertexBufferPtr->Color = color;
			_RendererData->QuadVertexBufferPtr->TexCoord = _RendererData->QuadTextureCoords[i];
			_RendererData->QuadVertexBufferPtr->TexIndex = texIndex;
			_RendererData->QuadVertexBufferPtr->TilingFactor = tilingFactor;
			_RendererData->QuadVertexBufferPtr++;
		}

		_RendererData->QuadIndexCount += 6;

		//_RendererData->TextureShader->SetFloat4(color, "u_Color");
		//_RendererData->TextureShader->SetFloat(1.0f, "u_TilingFactor");
		//_RendererData->WhiteTexture->Bind();

		/*auto transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		_RendererData->TextureShader->SetMat4(transform, "u_Transform");*/

		//_RendererData->QuadVertexArray->Bind();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		UR_PROFILE_SCOPE("LayerStack OnImGuiRender");

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < _RendererData->TextureSlotIndex; i++) {
			if (*_RendererData->TextureSlots[i].get() == *texture.get()) {
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = _RendererData->TextureSlotIndex;
			_RendererData->TextureSlots[_RendererData->TextureSlotIndex] = texture;
			_RendererData->TextureSlotIndex++;
		}

		const glm::vec4 color = glm::vec4(1.0f);

		_RendererData->QuadVertexBufferPtr->Position = position;
		_RendererData->QuadVertexBufferPtr->Color = color;
		_RendererData->QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		_RendererData->QuadVertexBufferPtr->TexIndex = textureIndex;
		_RendererData->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_RendererData->QuadVertexBufferPtr++;

		_RendererData->QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		_RendererData->QuadVertexBufferPtr->Color = color;
		_RendererData->QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		_RendererData->QuadVertexBufferPtr->TexIndex = textureIndex;
		_RendererData->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_RendererData->QuadVertexBufferPtr++;

		_RendererData->QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		_RendererData->QuadVertexBufferPtr->Color = color;
		_RendererData->QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		_RendererData->QuadVertexBufferPtr->TexIndex = textureIndex;
		_RendererData->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_RendererData->QuadVertexBufferPtr++;

		_RendererData->QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		_RendererData->QuadVertexBufferPtr->Color = color;
		_RendererData->QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		_RendererData->QuadVertexBufferPtr->TexIndex = textureIndex;
		_RendererData->QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_RendererData->QuadVertexBufferPtr++;

		_RendererData->QuadIndexCount += 6;

		/*_RendererData->TextureShader->SetFloat4(glm::vec4(1.0f), "u_Color");
		_RendererData->TextureShader->SetFloat(tilingFactor, "u_TilingFactor");
		texture->Bind();
		
		auto transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		_RendererData->TextureShader->SetMat4(transform, "u_Transform");

		_RendererData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_RendererData->QuadVertexArray);*/
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
