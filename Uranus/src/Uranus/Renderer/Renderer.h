#pragma once

namespace Uranus {

	enum class RendererAPI {
		None = 0,
		OpenGL = 1
	};

	class Renderer {

	public:
		Renderer();

	public:
		inline static RendererAPI GetApi() { return _RendererAPI; }
	private:
		static RendererAPI _RendererAPI;
	};
}