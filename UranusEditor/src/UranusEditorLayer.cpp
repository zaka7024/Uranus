#include "UranusEditorLayer.h"

#include "Uranus/Scene/SceneSerializer.h"
#include "Uranus/Utils/PlatformUtils.h"
#include "Uranus.h"

#include "ImGuizmo/ImGuizmo.h"
#include "Uranus/Math/Math.h"

#include <imgui\imgui.cpp>
#include <glm\glm\gtc\type_ptr.hpp>

#include <chrono>

namespace Uranus {

    UranusEditorLayer::UranusEditorLayer() : Layer("UranusEditorLayer"), _CameraController(1280.0f / 720.0f, true)
    {
        _EditorCamera = EditorCamera(30.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
    }

    void UranusEditorLayer::OnAttach()
    {
        Uranus::FramebufferSpecification framebufferSpecification;
        framebufferSpecification.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::DEPTH24STENCIL8 };
        framebufferSpecification.Height = 1280;
        framebufferSpecification.Width = 720;
        _FrameBuffer = Uranus::FrameBuffer::Create(framebufferSpecification);

        _ActiveScene = CreateRef<Scene>();

#if 0
        Entity squareEntity = _ActiveScene->CreateEntity("Square");

        _SquareEntity = squareEntity;
        auto& sr = _SquareEntity.AddComponent<SpriteRendererComponent>();

        _MainCamera = _ActiveScene->CreateEntity("Main Camera");
        _MainCamera.AddComponent<CameraComponent>();

        _SecondCamera = _ActiveScene->CreateEntity("Second Camera");
        auto& cc = _SecondCamera.AddComponent<CameraComponent>();
        cc.Primary = false;

        auto& redSquare = _ActiveScene->CreateEntity("Red Square");
        redSquare.AddComponent<SpriteRendererComponent>().Color = { 0.8f, 0.3f, 0.6f, 1.0f };

        class CameraController : public ScriptableEntity {
        public:
            void OnCreate() 
            {
                std::cout << "CameraController::OnCreate" << std::endl;
                transformComponent = &GetComponent<TransformComponent>();
            };

            void OnDestroy()
            {

            };

            void OnUpdate(Timestep ts)
            {
                auto& position = transformComponent->Translation;
                if (Input::IsKeyPressed(UR_KEY_A))
                    position.x -= cameraMoveSpeed * ts;
                if (Input::IsKeyPressed(UR_KEY_D))
                    position.x += cameraMoveSpeed * ts;
                if (Input::IsKeyPressed(UR_KEY_S))
                    position.y -= cameraMoveSpeed * ts;
                if (Input::IsKeyPressed(UR_KEY_W))
                    position.y += cameraMoveSpeed * ts;

            };

        private:
            float cameraMoveSpeed = 5.0f;
            TransformComponent* transformComponent;
        };

        _MainCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

#endif
        //
        _SceneHierarchyPanel.SetContext(_ActiveScene);
    }

    void UranusEditorLayer::OnDetach()
    {

    }

    void UranusEditorLayer::OnUpdate(Uranus::Timestep ts)
    {
        UR_PROFILE_FUNCTION();

        // Frame Buffer Resize
        if (FramebufferSpecification spec = _FrameBuffer->GetSpecification();
            _ViewportSize.x > 0.0f && _ViewportSize.y > 0.0f &&
            (spec.Width != _ViewportSize.x || spec.Height != _ViewportSize.y))
        {
            _FrameBuffer->Resize((uint32_t)_ViewportSize.x, (uint32_t)_ViewportSize.y);
            _CameraController.OnResize(_ViewportSize.x, _ViewportSize.y);
            _EditorCamera.SetViewportSize(_ViewportSize.x, _ViewportSize.y);
            _ActiveScene->OnViewportResize((uint32_t)_ViewportSize.x, (uint32_t)_ViewportSize.y);
        }

        {
            UR_PROFILE_SCOPE("CameraController::OnUpdate");
            if (_viewportFocused) {
                _CameraController.OnUpdate(ts);
            }

            _EditorCamera.OnUpdate(ts);
        }

        {
            UR_PROFILE_SCOPE("Renderer Prep");
            _FrameBuffer->Bind();
            Uranus::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
            Uranus::RenderCommand::Clear();

            // Clear entity id buffer
            _FrameBuffer->ClearAttachment(1, -1);
        }

        Uranus::Renderer2D::ResetStats();

        {
            UR_PROFILE_SCOPE("Renderer Draw");
            _ActiveScene->OnUpdateEditor(ts, _EditorCamera);
        }

        auto [mx, my] = ImGui::GetMousePos();
        mx -= _ViewportBounds[0].x;
        my -= _ViewportBounds[0].y;

        glm::vec2 viewportSize = _ViewportBounds[1] - _ViewportBounds[0];
        my = viewportSize.y - my;
        int mouseX = (int)mx;
        int mouseY = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
            int pixelData = _FrameBuffer->ReadPixel(1, mouseX, mouseY);
            pixelData != -1 && pixelData >= 0
            ? _HoveredEntity = Entity{ (entt::entity)pixelData, _ActiveScene.get() }
            : _HoveredEntity = Entity();
            UR_CORE_WARN("Pixel data = {0}", pixelData);
        }

        _FrameBuffer->Ubnind();
    }

    void UranusEditorLayer::OnEvent(Uranus::Event& event)
    {
        _EditorCamera.OnEvent(event);
        _CameraController.OnEvent(event);
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(UR_BIND_EVENT_FUN(UranusEditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(UR_BIND_EVENT_FUN(UranusEditorLayer::OnMouseButtonPressed));
    }

    bool UranusEditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        if (e.GetRepeatCount() > 0)
            return false;

        bool control = Input::IsKeyPressed(UR_KEY_LEFT_CONTROL) || Input::IsKeyPressed(UR_KEY_RIGHT_CONTROL);
        bool shift = Input::IsKeyPressed(UR_KEY_LEFT_SHIFT) || Input::IsKeyPressed(UR_KEY_RIGHT_SHIFT);
        switch (e.GetKeyCode())
        {
            case UR_KEY_N:
            {
                if (control)
                    NewScene();

                break;
            }
            case UR_KEY_O:
            {
                if (control)
                    OpenScene();

                break;
            }
            case UR_KEY_S:
            {
                if (control && shift)
                    SaveSceneAs();

                break;
            }

            case UR_KEY_Q:
                _GizmoType = -1;
                break;
            case UR_KEY_W:
                _GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            case UR_KEY_E:
                _GizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            case UR_KEY_R:
                _GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
        }
    }

    bool UranusEditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (e.GetMouseButton() == MouseButtons::ButtonLeft) {
            if (_viewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(KeyCode::LeftAlt))
                _SceneHierarchyPanel.SetSelectedEntity(_HoveredEntity);
        }
        return false;
    }

    void UranusEditorLayer::NewScene()
    {
        _ActiveScene = CreateRef<Scene>();
        _ActiveScene->OnViewportResize((uint32_t)_ViewportSize.x, (uint32_t)_ViewportSize.y);
        _SceneHierarchyPanel.SetContext(_ActiveScene);
    }

    void UranusEditorLayer::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("Uranus Scene (*.uranus)\0*.uranus\0");
        if (!filepath.empty())
        {
            _ActiveScene = CreateRef<Scene>();
            _ActiveScene->OnViewportResize((uint32_t)_ViewportSize.x, (uint32_t)_ViewportSize.y);
            _SceneHierarchyPanel.SetContext(_ActiveScene);

            SceneSerializer serializer(_ActiveScene);
            serializer.Deserialize(filepath);
        }
    }

    void UranusEditorLayer::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("Uranus Scene (*.uranus)\0*.uranus\0");
        if (!filepath.empty())
        {
            SceneSerializer serializer(_ActiveScene);
            serializer.Serialize(filepath);
        }
    }

    void UranusEditorLayer::OnImGuiRender()
    {
        static bool open = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {

                if (ImGui::MenuItem("New", "Ctrl+N"))
                    NewScene();

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                    OpenScene();

                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                    SaveSceneAs();

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }


        _SceneHierarchyPanel.OnImGuiRender();

        ImGui::Begin("Settings");

        ImGui::Text("Stats");
        auto stats = Uranus::Renderer2D::GetStats();
        ImGui::Text("Draw	Calls %d", stats.DrawCalls);
        ImGui::Text("Quad	Count %d", stats.QuadCount);
        ImGui::Text("Vertex Count %d", stats.GetTotalVertexCount());
        ImGui::Text("Index	Count %d", stats.GetTotalIndexCount());

        std::string name = "None";
        if (_HoveredEntity)
            name = _HoveredEntity.GetComponent<TagComponent>().Tag;
        ImGui::Text("Hovered Entity: %s", name.c_str());

        ImGui::End();
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
        ImGui::Begin("Viewport");
        auto viewportOffset = ImGui::GetCursorPos();
        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();

        auto windowSize = ImGui::GetWindowSize();
        ImVec2 minBound = ImGui::GetWindowPos();
        minBound.x += viewportOffset.x;
        //minBound.y += viewportOffset.y;

        ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
        _ViewportBounds[0] = { minBound.x, minBound.y };
        _ViewportBounds[1] = { maxBound.x, maxBound.y };

        _viewportFocused = ImGui::IsWindowFocused();
        _viewportHovered = ImGui::IsWindowHovered();

        Application::Get().GetImGuiLayer()->BlockEvents(!_viewportFocused && !_viewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        // Gizmos
        Entity selectedEntity = _SceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity && _GizmoType != -1)
        {
            ImGuizmo::SetOrthographic(false);
            //ImGuizmo::SetDrawlist();

            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            // Runtime Camera
            //auto cameraEntity = _ActiveScene->GetPrimaryCameraEntity();
            //auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
            //const glm::mat4& cameraProjection = camera.GetProjection();
            //glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

            // Editor Camera
            glm::mat4 cameraView = _EditorCamera.GetViewMatrix();
            

            // Entity transform
            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.GetTransform();

            // Snapping
            bool snap = Input::IsKeyPressed(UR_KEY_LEFT_CONTROL);
            float snapValue = 0.5f; // Snap to 0.5m for translation/scale
            // Snap to 45 degrees for rotation
            if (_GizmoType == ImGuizmo::OPERATION::ROTATE)
                snapValue = 45.0f;

            float snapValues[3] = { snapValue, snapValue, snapValue };

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(_EditorCamera.GetProjection()),
                (ImGuizmo::OPERATION)_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                nullptr, snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation, rotation, scale;
                Math::DecomposeTransform(transform, translation, rotation, scale);

                glm::vec3 deltaRotation = rotation - tc.Rotation;
                tc.Translation = translation;
                tc.Rotation += deltaRotation;
                tc.Scale = scale;
            }
        }

        ImGui::Image((void*)_FrameBuffer->GetColorAttachmentRendererId(0), { _ViewportSize.x, _ViewportSize.y}, { 0, 1 }, { 1, 0 });
        ImGui::PopStyleVar();
        ImGui::End();

        ImGui::End();
    }
}
