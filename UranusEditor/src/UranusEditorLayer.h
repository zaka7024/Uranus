#pragma once
#include "Uranus.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

namespace Uranus {

	class UranusEditorLayer : public Uranus::Layer {
	public:
		UranusEditorLayer();
		~UranusEditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Uranus::Timestep ts) override;
		virtual void OnEvent(Uranus::Event& event) override;
		virtual void OnImGuiRender() override;

	private:
		void NewScene();
		void OpenScene(const std::filesystem::path& path);
		void OpenScene();
		void SaveSceneAs();

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

	private:
		Uranus::OrthographicCameraController _CameraController;
		Uranus::Ref<Uranus::FrameBuffer> _FrameBuffer;
		Uranus::Ref<Uranus::Scene> _ActiveScene;


		Uranus::Entity _MainCamera;
		Uranus::Entity _SecondCamera;
		Uranus::Entity _SquareEntity;

		EditorCamera _EditorCamera;

		Entity _HoveredEntity;

		// panels
		SceneHierarchyPanel _SceneHierarchyPanel;
		ContentBrowserPanel _ContentBrowserPanel;
		int _GizmoType = -1;

		bool _PrimaryCamera = true;
		bool _viewportFocused = false, _viewportHovered = false;

		glm::vec2 _ViewportSize = { 0.0f, 0.0f };
		glm::vec2 _ViewportBounds[2];

		glm::vec4 _Color = { 0.4f, 0.2f, 0.7f, 1.0f };
		glm::vec3 _Position = { 0.0f, 0.0f, 0.1f };
		glm::vec2 _Scale = { 1.0f, 1.0f };

		float _Rotation = { 0.0f };
	};
}
