#pragma once

#include "Uranus/Renderer/RendererAPI.h"

namespace Uranus {

	class OpenGLRendererAPI : public RendererAPI {
	public:
		OpenGLRendererAPI() {};

		virtual void Init() override;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count) override;

	};
}