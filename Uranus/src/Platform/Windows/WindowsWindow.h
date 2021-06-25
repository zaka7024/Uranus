#pragma once

#include "Uranus/Core/Window.h"
#include "Uranus/Renderer/GraphicsContext.h"
#include "GLFW/glfw3.h"

namespace Uranus {

	class WindowsWindow : public Window {
	public:

		WindowsWindow(const WindowProps& windowProps);
		~WindowsWindow();

		void OnUpdate() override;


		inline unsigned int GetWidth() const override{ return _WindowData.Width; };
		inline unsigned int GetHeight() const override { return _WindowData.Height; }

		inline void SetEventCallback(const EventCallbackFun& callback) override { _WindowData.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() override;

		virtual void* GetNativeWindow() { return _Window; };

	private:

		void Init(const WindowProps& windowProps);
		void Shutdown();

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFun EventCallback;
		};

		Scope<GraphicsContext> _Context;

		WindowData _WindowData;
		GLFWwindow* _Window;
	};
}