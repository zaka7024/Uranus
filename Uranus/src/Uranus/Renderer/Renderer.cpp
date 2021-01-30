#include "urpch.h"

#include "Renderer.h"

namespace Uranus {

	std::unique_ptr<Renderer::SceneData> Renderer::_SceneData = std::make_unique<SceneData>();

	void Renderer::BeginScene(OrthographicCamera& camera) {
		_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform) {
		shader->Bind();
		shader->UploadUniformMat4(_SceneData->ViewProjectionMatrix, "u_ViewProjection");
		shader->UploadUniformMat4(transform, "u_Transfrom");

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
