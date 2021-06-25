#include "urpch.h"

#include "Renderer.h"
#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Uranus {

	Scope<Renderer::SceneData> Renderer::_SceneData = std::make_unique<SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(std::uint32_t width, std::uint32_t height)
	{
		RenderCommand::SetViewPort(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera) {
		_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const Uranus::Ref<Shader>& shader, const Uranus::Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
		shader->Bind();
		std::dynamic_pointer_cast<Uranus::OpenGLShader>(shader)->UploadUniformMat4(_SceneData->ViewProjectionMatrix, "u_ViewProjection");
		std::dynamic_pointer_cast<Uranus::OpenGLShader>(shader)->UploadUniformMat4(transform, "u_Transfrom");

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
