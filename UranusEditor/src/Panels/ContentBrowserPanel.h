#pragma once

#include <filesystem>

namespace Uranus {

	class ContentBrowserPanel {
	public:
		ContentBrowserPanel();

	public:
		void OnImGuiRender();

	private:
		std::filesystem::path _CurrentDirectory;
	};
}
