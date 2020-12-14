
#include "urpch.h"

#include "Application.h"

namespace Uranus {

	Uranus::Application::Application()
	{

	}

	Uranus::Application::~Application()
	{

	}

	void Uranus::Application::Run()
	{
		// Test
		WindowResizeEvent e(100, 100);
		UR_TRACE(e);

		while (true);
	}
}