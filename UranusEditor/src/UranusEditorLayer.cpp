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

            _ActiveScene->OnViewportResize((uint32_t)_ViewportSize.x, (uint32_t)_ViewportSize.y);
        }

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
                if (ImGui::MenuItem("Close", NULL, false, open != NULL)) {
                    Uranus::Application::Get().Close();
                    open = false;
                }
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

        ImGui::End();
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
        ImGui::Begin("Viewport");
        _viewportFocused = ImGui::IsWindowFocused();
        _viewportHovered = ImGui::IsWindowHovered();

        Application::Get().GetImGuiLayer()->BlockEvents(!_viewportFocused || !_viewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };


        ImGui::Image((void*)_FrameBuffer->GetColorAttachmentRendererId(), { _ViewportSize.x, _ViewportSize.y}, { 0, 1 }, { 1, 0 });
        ImGui::PopStyleVar();
        ImGui::End();

        ImGui::End();
    }
}
