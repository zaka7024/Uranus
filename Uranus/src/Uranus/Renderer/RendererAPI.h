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

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Uranus::Ref<VertexArray>& vertexArray) = 0;

		static API GetApi() { return _API; }

	private:
		static API _API;
	};
}