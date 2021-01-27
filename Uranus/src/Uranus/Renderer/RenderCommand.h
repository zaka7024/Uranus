#pragma once

#include "RendererAPI.h"

namespace Uranus {

	class RenderCommand {
	public:
		static void SetClearColor(const glm::vec4& color) { _RendererAPI->SetClearColor(color); }
		static void Clear() { _RendererAPI->Clear(); }
		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
			_RendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static RendererAPI* _RendererAPI;
	};
}