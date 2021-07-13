#include "SceneHierarchyPanel.h"
#include "Uranus/Scene/Components.h"

#include <imgui/imgui.h>

namespace Uranus {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		
		_Context->_Registry.each([&](auto entityId)
		{
			Entity entity(entityId, _Context.get());
			DrawEntityNode(entity);
		});

		ImGui::End();

		ImGui::Begin("Propertes Panel");
		if (_SelectionContext) {

			if (_SelectionContext.HasComponent<TagComponent>()) {
				auto& tag = _SelectionContext.GetComponent<TagComponent>().Tag;
				ImGui::Text(tag.c_str());
			}

		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = _Context->_Registry.get<TagComponent>(entity).Tag;

		ImGuiTreeNodeFlags flags = ((_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			_SelectionContext = entity;
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}
}