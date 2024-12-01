#include "UnlitRainbowMaterial.h"

UnlitRainbowMaterial::UnlitRainbowMaterial() : AMaterial(L"UnlitRainbowShader")
{

}

UnlitRainbowMaterial::~UnlitRainbowMaterial()
{

}

void UnlitRainbowMaterial::Render()
{

}

void UnlitRainbowMaterial::RenderUI()
{
	ImGui::Text("This shader will not an editor");
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}