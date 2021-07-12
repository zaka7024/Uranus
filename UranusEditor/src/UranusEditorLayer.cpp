#include "UranusEditorLayer.h"

#include <Uranus.h>

#include <imgui\imgui.cpp>
#include <glm\glm\gtc\type_ptr.hpp>

#include <chrono>

namespace Uranus {

    UranusEditorLayer::UranusEditorLayer() : Layer("UranusEditorLayer"), _CameraController(1280.0f / 720.0f, true)
    {

    }

    void UranusEditorLayer::OnAttach()
    {
        _CheckerboardTexture = Uranus::Texture2D::Create("assets/textures/Checkerboard.png");
        _PlayerTexture = Uranus::Texture2D::Create("assets/textures/moon.png");
        _TileTexture = Uranus::Texture2D::Create("assets/textures/tile.jpg");

        Uranus::FramebufferSpecification framebufferSpecification;
        framebufferSpecification.Height = 1280;
        framebufferSpecification.Width = 720;
        _FrameBuffer = Uranus::FrameBuffer::Create(framebufferSpecification);

        _ActiveScene = CreateRef<Scene>();
        Entity squareEntity = _ActiveScene->CreateEntity();

        _SquareEntity = squareEntity;
        _SquareEntity.AddComponent<SpriteRendererComponent>();

        _MainCamera = _ActiveScene->CreateEntity();
        _MainCamera.AddComponent<CameraComponent>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));

        _SecondCamera = _ActiveScene->CreateEntity();
        auto& cc = _SecondCamera.AddComponent<CameraComponent>(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));
        cc.Primary = false;
    }

    void UranusEditorLayer::OnDetach()
    {

    }

    void UranusEditorLayer::OnUpdate(Uranus::Timestep ts)
    {
        UR_PROFILE_FUNCTION();
        {
            UR_PROFILE_SCOPE("CameraController::OnUpdate");
            if (_viewportFocused) {
                _CameraController.OnUpdate(ts);
            }
        }

        {
            UR_PROFILE_SCOPE("Renderer Prep");
            _FrameBuffer->Bind();
            Uranus::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
            Uranus::RenderCommand::Clear();
        }

        Uranus::Renderer2D::ResetStats();

        {
            UR_PROFILE_SCOPE("Renderer Draw");
            _SquareEntity.GetComponent<SpriteRendererComponent>().Color = _Color;
            _ActiveScene->OnUpdate(ts);
        }

        _FrameBuffer->Ubnind();
    }

    void UranusEditorLayer::OnEvent(Uranus::Event& event)
    {
        _CameraController.OnEvent(event);
    }

    void UranusEditorLayer::OnImGuiRender()
    {
        static bool open = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
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

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
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
                if (ImGui::MenuItem("Close", NULL, false, open != NULL)) {
                    Uranus::Application::Get().Close();
                    open = false;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }


        ImGui::Begin("Color Picker");
        ImGui::SliderFloat3("Position", glm::value_ptr(_Position), -10, 10);
        ImGui::DragFloat("Rotation", &_Rotation, 0.1, 0, 360);
        ImGui::DragFloat2("Scale", glm::value_ptr(_Scale), 0.1, 0, 360);

        ImGui::Text("Stats");
        auto stats = Uranus::Renderer2D::GetStats();
        ImGui::Text("Draw	Calls %d", stats.DrawCalls);
        ImGui::Text("Quad	Count %d", stats.QuadCount);
        ImGui::Text("Vertex Count %d", stats.GetTotalVertexCount());
        ImGui::Text("Index	Count %d", stats.GetTotalIndexCount());

        if (_SquareEntity) {
            ImGui::Separator();
            ImGui::ColorEdit4("Color", glm::value_ptr(_Color));
            ImGui::Text("%s", _SquareEntity.GetComponent<TagComponent>().Tag.c_str());
            ImGui::Separator();
        }

        ImGui::DragFloat3("Camera Transform",
            glm::value_ptr(_MainCamera.GetComponent<TransformComponent>().Transform[3]));

        if (ImGui::Checkbox("Camera A", &_PrimaryCamera))
        {
            _MainCamera.GetComponent<CameraComponent>().Primary = _PrimaryCamera;
            _SecondCamera.GetComponent<CameraComponent>().Primary = !_PrimaryCamera;
        }

        ImGui::End();
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
        ImGui::Begin("Viewport");
        _viewportFocused = ImGui::IsWindowFocused();
        _viewportHovered = ImGui::IsWindowHovered();

        Application::Get().GetImGuiLayer()->BlockEvents(!_viewportFocused || !_viewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        
        if (_ViewportSize != *((glm::vec2*)(&viewportPanelSize)) && viewportPanelSize.x >= 0 && viewportPanelSize.y >= 0) {
            _FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
            _CameraController.OnResize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
            _ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        }

        ImGui::Image((void*)_FrameBuffer->GetColorAttachmentRendererId(), { _ViewportSize.x, _ViewportSize.y}, { 0, 1 }, { 1, 0 });
        ImGui::PopStyleVar();
        ImGui::End();

        ImGui::End();
    }
}
