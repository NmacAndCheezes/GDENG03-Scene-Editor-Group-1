#include "LitTextureMaterial.h"
#include "../GraphicsEngine.h"


LitTextureMaterial::LitTextureMaterial(std::string textureName) : AMaterial(L"LitTextureShader")
{
    texture = new Texture(GraphicsEngine::GetInstance(), textureName);
    texture->Init();

    sampler = new TextureSampler(GraphicsEngine::GetInstance());
    sampler->Init();
}

LitTextureMaterial::~LitTextureMaterial()
{
    texture->Release();
    sampler->Release();
}

void LitTextureMaterial::Render()
{
	texture->BindToPipeline();
	sampler->BindToPipeline();
}

void LitTextureMaterial::RenderUI()
{
    ImGui::Text("Texture:");

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 1.0f)); 
    if (ImGui::ImageButton("#AlbedoButton", (ImTextureID)(intptr_t)texture->GetTextureView(), ImVec2(50.f, 50.f)))
    {

    }
    ImGui::PopStyleVar();

    ImGui::Dummy(ImVec2(0.f, 5.f));
}