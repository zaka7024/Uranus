#pragma once

#include "RendererAPI.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "OrthographicCamera.h"

namespace Uranus {

	class Renderer {

	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(std::uint32_t width, std::uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void Submit(const Uranus::Ref<Shader>& shader, const Uranus::Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

	public:
		inline static RendererAPI::API GetApi() { return RendererAPI::GetApi(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> _SceneData;
	};
}