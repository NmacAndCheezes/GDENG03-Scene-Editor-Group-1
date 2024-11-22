#include "CreditsTab.h"
#include "GameEngine/Graphics/GraphicsEngine.h"


CreditsTab::CreditsTab() : AUITab(EditorGUIManager::TabNames::CREDITS_TAB.data())
{
	InitializeImGuiFlags();
	isEnabled = false;

	dlsuLogo = new Texture(GraphicsEngine::GetInstance(), "DLSU_Seal.png");
	dlsuLogo->Init(); 
}

CreditsTab::~CreditsTab() 
{
	dlsuLogo->Release();
}

void CreditsTab::InitializeImGuiFlags()
{
	flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking;
}

void CreditsTab::RenderUI()
{ 
	if (!isEnabled) return;

	ImGui::Begin(name.c_str(), &isEnabled, flags);

	float midPoint = ImGui::GetContentRegionAvail().x / 2.f;
	float offset = dlsuLogo->GetWidth() / 10.f;
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + midPoint - offset);
	ImGui::Image((ImTextureID)(intptr_t)dlsuLogo->GetTextureView(), ImVec2(dlsuLogo->GetWidth()/5.f, dlsuLogo->GetHeight()/5.f));
	ImGui::Dummy(ImVec2(0.f, 5.f));

	ImGui::Text("Scene Editor v.0.8.4");
	ImGui::Text("Developed by: Aamir C. Akim");
	ImGui::Dummy(ImVec2(0.f, 10.f));

	ImGui::Text("Projects used as reference:");
	ImGui::BulletText("GDENG03 Course"); 
	ImGui::BulletText("GDADPRG Course"); 
	ImGui::BulletText("ChiliTomatoNoodle C++ 3D DirectX Tutorial");
	ImGui::Dummy(ImVec2(0.f, 10.f));

	ImGui::Text("Libraries used in this project:");
	ImGui::BulletText("DirectX Tool Kit");
	ImGui::BulletText("Dear ImGui");
	ImGui::BulletText("stb_image.h by Sean T. Barrett and others");
	ImGui::BulletText("Open Asset Import Library");
	ImGui::BulletText("React Physics 3D");
	ImGui::Dummy(ImVec2(0.f, 10.f)); 

	ImGuiStyle& style = ImGui::GetStyle(); 
	midPoint = ImGui::GetContentRegionAvail().x / 2.f;
	offset = ImGui::CalcTextSize("Close").x / 2.f + style.FramePadding.x;
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + midPoint - offset); 
	if (ImGui::Button("Close"))
	{
		isEnabled = false;
	}

	ImGui::End(); 
}