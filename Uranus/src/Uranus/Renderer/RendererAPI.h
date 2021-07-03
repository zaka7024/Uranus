#pragma once
#include "glm/glm.hpp"

#include "VertexArray.h"

namespace Uranus {

	class RendererAPI {
	public:
		enum class API {
			None = 0,
			OpenGL = 1
		};

		virtual void Init() = 0;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Uranus::Ref<VertexArray>& vertexArray, uint32_t count) = 0;

		static API GetApi() { return _API; }

	private:
		static API _API;
	};
}