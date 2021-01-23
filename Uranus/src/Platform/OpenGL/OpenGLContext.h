#pragma once

#include "Uranus/Renderer/GraphicsContext.h"

namespace Uranus {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* _Window;
	};
}