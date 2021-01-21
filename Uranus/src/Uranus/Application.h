#pragma once

#include "urpch.h"

#include "Uranus/Core.h"
#include "Uranus/Log.h"
#include "Uranus/Events/Event.h"
#include "Uranus/Events/ApplicationEvent.h"
#include "Window.h"
#include "LayerStack.h"

namespace Uranus {

	class URANUS_API Application
	{
	public:
		Application();
		~Application();

		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *_Instance; }

		inline Window& GetWindow() { return (*Get()._Window); }

	private:
		static Application* _Instance;
		std::unique_ptr<Window> _Window;
		bool _IsRunning = true;
		LayerStack _layerStack;

		bool OnWindowClose(WindowCloseEvent& e);
	};

	Application* CreateApplication();
}


