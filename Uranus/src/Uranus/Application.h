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

	private:
		std::unique_ptr<Window> _Window;
		bool _IsRunning = true;
		LayerStack _layerStack;

		bool onWindowClose(WindowCloseEvent& e);
	};

	Application* CreateApplication();
}


