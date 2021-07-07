#include "Sandbox2D.h"

#include <Uranus.h>

#include <imgui\imgui.cpp>
#include <glm\glm\gtc\type_ptr.hpp>

#include <chrono>


Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), _CameraController(1280.0f / 720.0f, true)
{

}

void Sandbox2D::OnAttach()
{
	_CheckerboardTexture = Uranus::Texture2D::Create("assets/textures/Checkerboard.png");
	_PlayerTexture = Uranus::Texture2D::Create("assets/textures/moon.png");
	_TileTexture = Uranus::Texture2D::Create("assets/textures/tile.jpg");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Uranus::Timestep ts)
{
	UR_PROFILE_FUNCTION();
	{
		UR_PROFILE_SCOPE("CameraController::OnUpdate");
		_CameraController.OnUpdate(ts);
	}

	{
		UR_PROFILE_SCOPE("Renderer Prep");
		Uranus::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Uranus::RenderCommand::Clear();
	}

	Uranus::Renderer2D::ResetStats();

	{
		UR_PROFILE_SCOPE("Renderer Draw");
		Uranus::Renderer2D::BeginScene(_CameraController.GetCamera());
		Uranus::Renderer2D::DrawQuad(glm::vec3(0.0f), { 10.0f, 10.0f }, _CheckerboardTexture, 10);
		
		static float angel;
		angel += ts * 120.0f;

		Uranus::Renderer2D::DrawRotatedQuad({-2.0f, 0.0f}, { 1.0f, 1.0f }, angel, _CheckerboardTexture, 10, { 1.0f, 0.8f, 0.8f, 1.0f });
		
		Uranus::Renderer2D::DrawQuad(_Position, { 1.0f, 1.0f }, _PlayerTexture, 1);
		
		Uranus::Renderer2D::DrawQuad({ 1.0f, 0.0f }, _Scale, _TileTexture);
		
		Uranus::Renderer2D::DrawRotatedQuad({ 1.0f, -1.0f }, _Scale, angel, {0.7, 0.2, 0.5, 1.0f});

		Uranus::Renderer2D::EndScene();

		Uranus::Renderer2D::BeginScene(_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.4f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.4f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.8f };
				Uranus::Renderer2D::DrawRotatedQuad({ x, y }, { 0.48f, 0.48f }, angel, color);
			}
		}
	}
	
	Uranus::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(Uranus::Event& event)
{
	_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Dock Space");
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
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false, open != NULL))
                open = false;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();
    ImGui::End();

	ImGui::Begin("Color Picker");
	ImGui::ColorEdit4("Color", glm::value_ptr(_Color));
	ImGui::SliderFloat3("Position", glm::value_ptr(_Position),-10, 10);
	ImGui::DragFloat("Rotation", &_Rotation, 0.1, 0, 360);
	ImGui::DragFloat2("Scale", glm::value_ptr(_Scale), 0.1, 0, 360);

	ImGui::Text("Stats");
	auto stats = Uranus::Renderer2D::GetStats();
	ImGui::Text("Draw	Calls %d", stats.DrawCalls);
	ImGui::Text("Quad	Count %d", stats.QuadCount);
	ImGui::Text("Vertex Count %d", stats.GetTotalVertexCount());
	ImGui::Text("Index	Count %d", stats.GetTotalIndexCount());
	ImGui::End();
}
