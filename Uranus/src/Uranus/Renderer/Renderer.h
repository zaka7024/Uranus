#pragma once

#include "RendererAPI.h"
#include "RenderCommand.h"

namespace Uranus {

	class Renderer {

	public:
		static void BeginScene();
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

	public:
		inline static RendererAPI::API GetApi() { return RendererAPI::GetApi(); }
	};
}