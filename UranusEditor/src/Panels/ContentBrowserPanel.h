#pragma once

#include <filesystem>

#include "Uranus/Renderer/Texture.h"

namespace Uranus {

	class ContentBrowserPanel {
	public:
		ContentBrowserPanel();

	public:
		void OnImGuiRender();

	private:
		std::filesystem::path _CurrentDirectory;

		Ref<Texture2D> _DirIcon;
		Ref<Texture2D> _FileIcon;
		Ref<Texture2D> _BackIcon;
	};
}
