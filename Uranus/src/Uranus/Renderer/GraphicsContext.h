#pragma once

struct GLFWwindow;
namespace Uranus {
	
	class GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	protected:
		GLFWwindow* _Window;
	};
}