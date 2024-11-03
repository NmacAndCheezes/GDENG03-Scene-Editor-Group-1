#include "EToolBar.h"
#include "EditorUIManager.h"
#include "../GameEngine/GameObjects/Primitives/CubeObject.h"
#include "../GameEngine/Managers/GameObjectManager.h"
EToolBar::EToolBar()
{
}

void EToolBar::update()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always); // Set position to top-left
    ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SameLine(); // Keep buttons on the same line
    ImGui::End();
}

EToolBar::~EToolBar()
{
}
