
#include "ContentBrowserPanel.h"
#include <imgui/imgui.h>

namespace Uranus {

	extern const std::filesystem::path s_AssetsPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
	:_CurrentDirectory(s_AssetsPath){
		_DirIcon = Texture2D::Create("Resources/Icons/dirIcon.png");
		_FileIcon = Texture2D::Create("Resources/Icons/fileIcon.png");
		_BackIcon = Texture2D::Create("Resources/Icons/backIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Assets");

		if (_CurrentDirectory != s_AssetsPath) {
			if (ImGui::ImageButton((ImTextureID)_BackIcon->GetRendereId(), { 32, 32 })) {
				_CurrentDirectory = _CurrentDirectory.parent_path();
			}
		}

		static float thumbnailSize = 64;
		static float padding = 16;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = panelWidth / cellSize;
		if (columnCount < 1) columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& it : std::filesystem::directory_iterator(_CurrentDirectory)) {
			std::string& filename = it.path().filename().string();

			ImGui::PushID(filename.c_str());
			Ref<Texture2D> icon = it.is_directory() ? _DirIcon : _FileIcon;

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0, 0, 0, 0});
			ImGui::ImageButton((ImTextureID)icon->GetRendereId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			
			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = it.path().c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (it.is_directory()) {

					_CurrentDirectory /= it.path().filename();
				}
			}
			ImGui::TextWrapped(filename.c_str());

			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::Columns(1);

		//ImGui::SliderFloat("thumbnailSize", &thumbnailSize, 64, 256);
		//ImGui::SliderFloat("padding", &padding, 0, 64);

		ImGui::End();
	}

}