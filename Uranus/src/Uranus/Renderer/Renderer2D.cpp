#include "urpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

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
		std::dynamic_pointer_cast<OpenGLShader>(_RendererData->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(_RendererData->FlatColorShader)->UploadUniformMat4(camera.GetViewProjectionMatrix(), "u_ViewProjection");
		std::dynamic_pointer_cast<OpenGLShader>(_RendererData->FlatColorShader)->UploadUniformMat4(glm::mat4(1.0f), "u_Transform");
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(_RendererData->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(_RendererData->FlatColorShader)->UploadUniformFloat4(color, "u_Color");

		_RendererData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(_RendererData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{

	}
}
