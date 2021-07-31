#pragma once
#include "Uranus.h"

#include "Panels/SceneHierarchyPanel.h"

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
		void OpenScene();
		void SaveSceneAs();

		bool OnKeyPressed(KeyPressedEvent& e);

	private:
		Uranus::OrthographicCameraController _CameraController;
		Uranus::Ref<Uranus::VertexArray> _SquareVA;
		Uranus::Ref<Uranus::Shader> _FlatColorShader;
		Uranus::Ref<Uranus::FrameBuffer> _FrameBuffer;
		Uranus::Ref<Uranus::Scene> _ActiveScene;

		Uranus::Ref<Uranus::Texture2D> _CheckerboardTexture;
		Uranus::Ref<Uranus::Texture2D> _PlayerTexture;
		Uranus::Ref<Uranus::Texture2D> _TileTexture;

		Uranus::Entity _MainCamera;
		Uranus::Entity _SecondCamera;
		Uranus::Entity _SquareEntity;

		EditorCamera _EditorCamera;

		// panels
		SceneHierarchyPanel _SceneHierarchyPanel;
		int _GizmoType = -1;

		bool _PrimaryCamera = true;
		bool _viewportFocused = false, _viewportHovered = false;

		glm::vec2 _ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		glm::vec4 _Color = { 0.4f, 0.2f, 0.7f, 1.0f };
		glm::vec3 _Position = { 0.0f, 0.0f, 0.1f };
		glm::vec2 _Scale = { 1.0f, 1.0f };

		float _Rotation = { 0.0f };
	};
}
