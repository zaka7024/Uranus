#pragma once

#include "RendererAPI.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "OrthographicCamera.h"

namespace Uranus {

	class Renderer {

	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

	public:
		inline static RendererAPI::API GetApi() { return RendererAPI::GetApi(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static std::unique_ptr<SceneData> _SceneData;
	};
}