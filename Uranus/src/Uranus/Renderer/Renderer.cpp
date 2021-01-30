#include "urpch.h"

#include "Renderer.h"

namespace Uranus {

	std::unique_ptr<Renderer::SceneData> Renderer::_SceneData = std::make_unique<SceneData>();

	void Renderer::BeginScene(OrthographicCamera& camera) {
		_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray) {
		shader->Bind();
		shader->UploadUniformMat4(_SceneData->ViewProjectionMatrix, "u_ViewProjection");

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
