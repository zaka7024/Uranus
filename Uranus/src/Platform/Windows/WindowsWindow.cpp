
#include "urpch.h"
#include "WindowsWindow.h"

#include "Uranus/Events/ApplicationEvent.h"
#include "Uranus/Events/KeyEvent.h"
#include "Uranus/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"


namespace Uranus {

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& windowProps) {
		return new WindowsWindow(windowProps);
	}

	WindowsWindow::WindowsWindow(const WindowProps& windowProps) {
		Init(windowProps);
	}

	WindowsWindow::~WindowsWindow()
	{
		delete _Window;
	}

	void WindowsWindow::OnUpdate() {
		_Context->SwapBuffers();
	}

	void WindowsWindow::Init(const WindowProps& windowProps) {
		UR_PROFILE_FUNCTION();

		_WindowData.Title = windowProps.Name;
		_WindowData.Height = windowProps.Height;
		_WindowData.Width = windowProps.Width;

		UR_CORE_INFO("Creating window {0} ({1}, {2})", _WindowData.Title, _WindowData.Height, _WindowData.Width);

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			UR_CORE_ASSERT(true, "Could not initilize GLFW!");
			s_GLFWInitialized = true;
		}

		_Window = glfwCreateWindow(_WindowData.Width, _WindowData.Height, _WindowData.Title.c_str(), nullptr, nullptr);
		
		// In the future we can switch to diffrenet graphics API
		_Context = CreateScope<OpenGLContext>(_Window);
		_Context->Init();

		glfwSetWindowUserPointer(_Window, &_WindowData);
		SetVSync(true);

		// Set GLFW events callbacks
		glfwSetWindowSizeCallback(_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			windowData.Width = width;
			windowData.Height = height;

			WindowResizeEvent event(width, height);
			windowData.EventCallback(event);
		});

		glfwSetWindowCloseCallback(_Window, [](GLFWwindow* window) {
			
			WindowData windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			windowData.EventCallback(event);
		});

		glfwSetKeyCallback(_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {

			WindowData windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS: 
				{
					KeyPressedEvent event(key, 0);
					windowData.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					windowData.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					windowData.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(_Window, [](GLFWwindow* window, unsigned int keyCode) {
			WindowData windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keyCode);
			windowData.EventCallback(event);
		});

		glfwSetMouseButtonCallback(_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event((MouseButtons)button);
					windowData.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonRelesedEvent event((MouseButtons)button);
					windowData.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			windowData.EventCallback(event);

		});

		glfwSetCursorPosCallback(_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData windowData = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			windowData.EventCallback(event);

		});
	}

	void WindowsWindow::Shutdown() {
		UR_PROFILE_FUNCTION();

		glfwDestroyWindow(_Window);
	}

	void WindowsWindow::SetVSync(bool enabled) {
		UR_PROFILE_FUNCTION();

		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}

		_WindowData.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() {
		return false;
	}
}