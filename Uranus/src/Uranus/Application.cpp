
#include "urpch.h"

#include "Application.h"

#include "glad/glad.h"

namespace Uranus {

#define BIND_EVENT_FUN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		_Window = std::unique_ptr<Window>(Window::Create());
		_Window->SetEventCallback(BIND_EVENT_FUN(OnEvent));
	}

	void Application::OnEvent(Event& e) {

		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUN(onWindowClose));

		// UR_CORE_TRACE("{0}", e);

		for (auto item = _layerStack.end(); item != _layerStack.begin(); ) {
			(*--item)->OnEvent(e);
			if (e.isHandled) break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		_layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		_layerStack.PushOverlay(layer);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (_IsRunning) {
			_Window->OnUpdate();

			for (Layer* layer : _layerStack)
				layer->OnUpdate();
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& e) {
		_IsRunning = false;
		return true;
	}
}