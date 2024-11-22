#include "ColorPickerTab.h"


ColorPickerTab::ColorPickerTab() : AUITab(EditorGUIManager::TabNames::COLOR_PICKER_TAB.data())
{
	InitializeImGuiFlags();
	isEnabled = false;
}

ColorPickerTab::~ColorPickerTab()
{

}

void ColorPickerTab::InitializeImGuiFlags()
{
	flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking;
}

void ColorPickerTab::RenderUI()
{
	if (ImGui::Begin(name.c_str(), &isEnabled, flags));

	float w = (500.f - ImGui::GetStyle().ItemSpacing.y) * 0.60f; 
	ImGui::SetNextItemWidth(w); 
	ImGui::ColorPicker3("##ColorPicker1", (float*)&color, ImGuiColorEditFlags_PickerHueBar 
		| ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

	ImGui::SetNextItemWidth(w); 
	ImGui::ColorPicker3("##ColorPicker2", (float*)&color, ImGuiColorEditFlags_PickerHueWheel 
		| ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

	ImGui::End(); 
}