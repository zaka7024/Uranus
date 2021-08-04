#include "SceneHierarchyPanel.h"
#include "Uranus/Scene/Components.h"
#include "Uranus/Scene/AssetsManager.h"

#include <imgui/imgui.h>
#include <glm\glm\gtc\type_ptr.hpp>

#include <filesystem>

namespace Uranus {

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		//ImGui::CalcItemWidth()
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		//float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { 20, 19 };
		const int dragFloatWidth = 45;

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);


		ImGui::PushItemWidth(dragFloatWidth);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushItemWidth(dragFloatWidth);
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushItemWidth(dragFloatWidth);
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T>
	static void DrawComponent(const std::string& name, Entity entity, std::function<void()> uiFunction)
	{
		const ImGuiTabBarFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::SameLine(ImGui::GetWindowWidth() - 30.0f);

		if (ImGui::Button("...")) {
			ImGui::OpenPopup("ComponenetSettings");
		}

		ImGui::PopStyleVar();

		bool componentRemoved = false;
		if (ImGui::BeginPopup("ComponenetSettings")) {
			if (ImGui::MenuItem("Remove Component"))
				componentRemoved = true;
			ImGui::EndPopup();
		}

		if (open) {
			uiFunction();
			ImGui::TreePop();
		}

		if (componentRemoved) {
			entity.RemoveComponent<T>();
		}
	}

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		_Context = context;
		_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		
		_Context->_Registry.each([&](auto entityId)
		{
			Entity entity(entityId, _Context.get());
			DrawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			_SelectionContext = {};

		if (ImGui::BeginPopupContextWindow(0, 1, false)) {
			if (ImGui::MenuItem("Create Entity")) {
				Entity entity = _Context->CreateEntity("Empty Entity");
				_SelectionContext = entity;
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Propertes Panel");
		if (_SelectionContext) {
			DrawComponents(_SelectionContext);
			
			ImGui::Separator();
			
			if (ImGui::Button("Add Component", { ImGui::GetContentRegionAvailWidth(), 20 }))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent")) {

				if (ImGui::MenuItem("Transform")) {
					if (!_SelectionContext.HasComponent<TransformComponent>())
						_SelectionContext.AddComponent<TransformComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Camera")) {
					if (!_SelectionContext.HasComponent<CameraComponent>())
						_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Renderer")) {
					if (!_SelectionContext.HasComponent<SpriteRendererComponent>())
						_SelectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[255];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		}	

		const ImGuiTabBarFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<TransformComponent>()) {
			DrawComponent<TransformComponent>("Transform", entity, [&]() 
			{
					auto& translation = entity.GetComponent<TransformComponent>().Translation;
					auto& rotation = entity.GetComponent<TransformComponent>().Rotation;
					auto& scale = entity.GetComponent<TransformComponent>().Scale;

					DrawVec3Control("Position", (translation));

					auto rotationInDegrees = glm::degrees(rotation);
					DrawVec3Control("Rotation", rotationInDegrees);
					rotation = glm::radians(rotationInDegrees);

					DrawVec3Control("Scale", (scale), 1.0f);
			});
		}

		if (entity.HasComponent<CameraComponent>()) {
			DrawComponent<CameraComponent>("Camera", entity, [&]() 
			{
					auto& cameraComponent = entity.GetComponent<CameraComponent>();
					auto& camera = cameraComponent.Camera;

					ImGui::Checkbox("Primary", &cameraComponent.Primary);

					const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
					const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

					if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
						for (int i = 0; i < 2; i++)
						{
							if (ImGui::Selectable(projectionTypeStrings[i])) {
								currentProjectionTypeString = projectionTypeStrings[i];
								camera.SetProjectionType((SceneCamera::ProjectionType)i);

								if (projectionTypeStrings[i] == currentProjectionTypeString) {
									ImGui::SetItemDefaultFocus();
								}
							}
						}

						ImGui::EndCombo();
					}

					if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
					{
						float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
						if (ImGui::DragFloat("Vertical FOV", &verticalFov))
							camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

						float orthoNear = camera.GetPerspectiveNearClip();
						if (ImGui::DragFloat("Near", &orthoNear))
							camera.SetPerspectiveNearClip(orthoNear);

						float orthoFar = camera.GetPerspectiveFarClip();
						if (ImGui::DragFloat("Far", &orthoFar))
							camera.SetPerspectiveFarClip(orthoFar);
					}

					if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
					{
						float orthoSize = camera.GetOrthographicSize();
						if (ImGui::DragFloat("Size", &orthoSize))
							camera.SetOrthographicSize(orthoSize);

						float orthoNear = camera.GetOrthographicNearClip();
						if (ImGui::DragFloat("Near", &orthoNear))
							camera.SetOrthographicNearClip(orthoNear);

						float orthoFar = camera.GetOrthographicFarClip();
						if (ImGui::DragFloat("Far", &orthoFar))
							camera.SetOrthographicFarClip(orthoFar);

						ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
					}
			});
		}

		if (entity.HasComponent<SpriteRendererComponent>()) {

			DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [&]() 
			{
				auto& src = entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(src.Color), 0.1f);

				if (src.Texture.get()) {
					ImGui::ImageButton((ImTextureID)src.Texture->GetRendereId(), { 64, 64 }, { 0, 1 }, { 1, 0 });
				}
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;

						// Todo:: Assets Manager
						//AssetsManager.AddAsset(Texture2D::Create(std::filesystem::path(path).string()));
						//AssetsManager.Lockup(filepath)
						auto filepath = std::filesystem::path(path);

						Ref<Asset> asset = AssetsManager::Lockup(filepath);
						if (!asset) {
							Ref<Texture2D> tex = Texture2D::Create(filepath.string());
							AssetsManager::AddAsset(tex);
						}

						Ref<Texture2D> texAsset = (Ref<Texture2D>)(Texture2D*)AssetsManager::Lockup(filepath).get();
						src.Texture = texAsset;
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::DragFloat("Tiling Factor", &src.TilingFactor, 0.1f, 0.0f, 100.0f);
			});
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = _Context->_Registry.get<TagComponent>(entity).Tag;

		ImGuiTreeNodeFlags flags = ((_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			_SelectionContext = entity;
		}

		bool entityDeleted = false;

		if (ImGui::BeginPopupContextItem(0, 1)) {
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			if (ImGui::MenuItem("Duplicate")) {
				Entity newEntity = _Context->CreateEntity("Empty Entity");
				newEntity.CopyFrom(entity);
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted) {
			_Context->DeleteEntity(entity);
			if (_SelectionContext == entity)
				_SelectionContext = {};
		}
	}
}