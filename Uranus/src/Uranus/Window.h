#pragma once

#include "urpch.h"

#include "Uranus/Events/Event.h"
#include "Uranus/Core.h"

namespace Uranus {

	struct WindowProps{
		std::string Name;
		unsigned int Width, Height;

		WindowProps(const std::string& name = "Uranus Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Name(name), Width(width), Height(height) {}
	};
	
	class URANUS_API Window {
	public:
		using EventCallbackFun = std::function<void(Event&)>;

		virtual ~Window() {};

		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFun& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() = 0;

		static Window* Create(const WindowProps& WindowProps = WindowProps());
	};
}