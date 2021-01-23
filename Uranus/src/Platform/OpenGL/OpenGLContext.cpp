#include "urpch.h"

#include "OpenGLContext.h"
#include "Uranus/Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Uranus {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:_Window(windowHandle)
	{

	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(_Window);
		// Initilize Glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		UR_CORE_ASSERT(status, "Could not initilize Glad!")
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwPollEvents();
		glfwSwapBuffers(_Window);
	}
}
