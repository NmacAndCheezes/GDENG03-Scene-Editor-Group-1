#include "DockSpace.h"

DockSpace::DockSpace() : AUITab(EditorGUIManager::TabNames::DOCKSPACE.data()), dockSpaceFlags(0)
{
	InitializeImGuiFlags();
}

DockSpace::~DockSpace()
{

}

void DockSpace::InitializeImGuiFlags()
{
	flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNavFocus;
    flags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;
    dockSpaceFlags |= ImGuiDockNodeFlags_PassthruCentralNode;
}

void DockSpace::RenderUI()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 size = ImVec2(viewport->WorkSize.x + 20.f, viewport->WorkSize.y + 20.f);
    ImGui::SetNextWindowPos(ImVec2(-10.0f, 10.0f));
    ImGui::SetNextWindowSize(size);  
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::Begin(name.c_str(), &isEnabled, flags);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace"); 
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockSpaceFlags);

    ImGui::End();
}