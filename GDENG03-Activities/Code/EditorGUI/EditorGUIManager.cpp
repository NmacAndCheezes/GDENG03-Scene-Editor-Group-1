#include "EditorGUIManager.h"
#include "GameEngine/Graphics/GraphicsEngine.h"

#include "DockSpace.h"
#include "CreditsTab.h"
#include "MenuTab.h"
#include "HierarchyTab.h"
#include "InspectorTab.h"
#include "DebugLogUI.h"
#include "ColorPickerTab.h"


EditorGUIManager* EditorGUIManager::sharedInstance = nullptr;

EditorGUIManager* EditorGUIManager::GetInstance()
{
	if (sharedInstance == nullptr)
	{
		sharedInstance = new EditorGUIManager();
	}

	return sharedInstance;
}

void EditorGUIManager::Destroy()
{
	if (!sharedInstance) return;

	for (auto keyValuePair : sharedInstance->uiTable) 
	{
		AUITab* ui = keyValuePair.second; 
		if (ui) delete ui;
	}
	sharedInstance->uiTable.clear();
	//sharedInstance->uiList.clear();
	//sharedInstance->uiList.shrink_to_fit();

	//ImGui::DestroyContext(); 
	delete sharedInstance;
}

bool EditorGUIManager::Initialize(HWND hWnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd); 
	ImGui_ImplDX11_Init(GraphicsEngine::GetInstance()->GetDevice(), GraphicsEngine::GetInstance()->GetDeviceContext()); 

	DockSpace* dockSpace = new DockSpace();
	uiTable[dockSpace->Name] = dockSpace;

	CreditsTab* credits = new CreditsTab();
	uiTable[credits->Name] = credits;

	MenuTab* menu = new MenuTab();
	uiTable[menu->Name] = menu;

	HierarchyTab* hierarchy = new HierarchyTab();
	uiTable[hierarchy->Name] = hierarchy;

	InspectorTab* inspector = new InspectorTab(hierarchy);
	uiTable[inspector->Name] = inspector;

	DebugLogUI* debug = new DebugLogUI();
	uiTable[debug->Name] = debug;

	ColorPickerTab* colorPicker = new ColorPickerTab(); 
	uiTable[colorPicker->Name] = colorPicker; 

	return true;
}

void EditorGUIManager::Render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	/*for (auto tab : uiList)
	{
		if (tab && tab->Enabled) tab->Render();
	}*/

	for (auto keyValuePair : uiTable)
	{
		AUITab* ui = keyValuePair.second;
		if (ui && ui->Enabled) ui->RenderUI();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();
}

AUITab* EditorGUIManager::GetTab(std::string tabName)
{
	return uiTable[tabName];
}

bool EditorGUIManager::IsMousesOverUI()
{
	return ImGui::GetIO().WantCaptureMouse; 
}
