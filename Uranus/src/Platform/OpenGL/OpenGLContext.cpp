#include "urpch.h"

#include "OpenGLContext.h"
#include "Uranus/Core/Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Uranus {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:_Window(windowHandle)
	{
	}

	void OpenGLContext::Init()
	{
		UR_PROFILE_FUNCTION();

		glfwMakeContextCurrent(_Window);
		// Initilize Glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		UR_CORE_ASSERT(status, "Could not initilize Glad!")


		UR_CORE_INFO("OpenGL Info:");
		UR_CORE_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		UR_CORE_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		UR_CORE_INFO("Version: {0}", glGetString(GL_VERSION));

		int width, height;
		glfwGetWindowSize(_Window, &width, &height);
		glViewport(0, 0, 1280, 720);
	}

	void OpenGLContext::SwapBuffers()
	{
		UR_PROFILE_FUNCTION();

		glfwPollEvents();
		glfwSwapBuffers(_Window);
	}
}
