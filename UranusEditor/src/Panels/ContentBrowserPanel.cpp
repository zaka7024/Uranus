
#include "ContentBrowserPanel.h"
#include <imgui/imgui.h>

namespace Uranus {

	static std::filesystem::path s_AssetsPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
	:_CurrentDirectory(s_AssetsPath){
		
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Assets");

		if (_CurrentDirectory != s_AssetsPath) {
			if (ImGui::Button("<-")) {
				_CurrentDirectory = _CurrentDirectory.parent_path();
			}
		}

		for (auto& it : std::filesystem::directory_iterator(_CurrentDirectory)) {

			if (it.is_directory()) {
				if (ImGui::Button(it.path().filename().string().c_str())) {
					_CurrentDirectory /= it.path().filename();
				}
			}
			else {
				ImGui::Button(it.path().filename().string().c_str());
			}

			
		}

		ImGui::End();
	}

}