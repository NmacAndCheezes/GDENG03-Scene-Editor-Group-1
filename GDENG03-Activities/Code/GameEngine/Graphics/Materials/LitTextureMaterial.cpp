#include "LitTextureMaterial.h"
#include "../GraphicsEngine.h"
#include "WindowSystem/FileDialogs.h"
#include <sstream>


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
        const char* supportedImageFormats =  
            "PNG (*.png)\0*.png\0"
            "JPG (*.jpg)\0*.jpg\0"
            "JPEG (*.jpeg)\0*.jpeg\0"
        ;
        std::string file_path = FileDialogs::OpenFile(supportedImageFormats);

        if (!file_path.empty())
        {
            std::stringstream ss = std::stringstream(file_path);
            std::vector<std::string> tempList;
            std::string temp;
            while(std::getline(ss, temp, '\\'))
            {
                tempList.push_back(temp);
            }

            texture->Release(); 
            texture = new Texture(GraphicsEngine::GetInstance(), tempList.back()); 
            texture->Init(); 
        }
    }
    ImGui::PopStyleVar();

    ImGui::Dummy(ImVec2(0.f, 5.f));
}