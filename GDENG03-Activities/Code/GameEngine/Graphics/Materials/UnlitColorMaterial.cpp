#include "UnlitColorMaterial.h"


UnlitColorMaterial::UnlitColorMaterial(const Vector3& color) : AMaterial(L"UnlitColorShader"), color(color)
{
	pixelColorBuffer = new PixelConstantBuffer<UnlitColor>(GraphicsEngine::GetInstance(), 2u);
	pixelColorBuffer->Init();
}

UnlitColorMaterial::~UnlitColorMaterial()
{
	if (pixelColorBuffer)
	{
		pixelColorBuffer->Release();
	}
}

void UnlitColorMaterial::Render()
{
	UnlitColor colorBuffer = { color };
	pixelColorBuffer->SetConstants(colorBuffer);
	pixelColorBuffer->BindToPipeline();
}

void UnlitColorMaterial::RenderUI()
{
	ImGui::Text("Albedo Color:");
	ImGui::SameLine();

	ImGuiColorEditFlags colorFlags = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueWheel | 
		ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoAlpha;
	ImGui::ColorEdit3("##MaterialColorPicker", (float*)&color, colorFlags);
}

void UnlitColorMaterial::SetColor(const Vector3& newColor)
{
	color = newColor;
}

Vector3& UnlitColorMaterial::GetColor()
{
	return color;
}
