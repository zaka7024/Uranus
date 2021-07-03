#pragma once

#include "RendererAPI.h"

namespace Uranus {

	class RenderCommand {
	public:
		static void Init() { _RendererAPI->Init(); }
		static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			_RendererAPI->SetViewPort(x, y, width, height);
		}
		static void SetClearColor(const glm::vec4& color) { _RendererAPI->SetClearColor(color); }
		static void Clear() { _RendererAPI->Clear(); }
		static void DrawIndexed(const Uranus::Ref<VertexArray>& vertexArray, uint32_t count = 0) {
			_RendererAPI->DrawIndexed(vertexArray, count);
		}

	private:
		static Scope<RendererAPI> _RendererAPI;
	};
}