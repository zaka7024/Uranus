#pragma once

#include "Uranus/Core.h"

#include "Uranus/Events/ApplicationEvent.h"
#include "Uranus/Log.h"

namespace Uranus {

	class URANUS_API Application
	{
	public:
		Application();
		~Application();

		void Run();
	};

	Application* CreateApplication();
}


