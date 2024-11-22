#pragma once
#include <imgui.h>
#include <Windows.h>
#include <unordered_map>
#include <string>


class AUITab;

class EditorGUIManager
{
public:
	class TabNames
	{
	public:
		static constexpr std::string_view DOCKSPACE = "DockSpace";
		static constexpr std::string_view MENU_TAB = "MenuBar";
		static constexpr std::string_view CREDITS_TAB = "Credits"; 
		static constexpr std::string_view HIERARCHY_TAB = "Hierarchy";
		static constexpr std::string_view INSPECTOR_TAB = "Inspector";
		static constexpr std::string_view CONSOLE_TAB = "Console";
		static constexpr std::string_view COLOR_PICKER_TAB = "Color Picker";
	};

public:
	static EditorGUIManager* GetInstance(); 
	static void Destroy();
	EditorGUIManager(const EditorGUIManager&) = delete;
	EditorGUIManager& operator=(const EditorGUIManager&) = delete; 

	bool Initialize(HWND hWnd);
	void Render();

	AUITab* GetTab(std::string tabName);
	bool IsMousesOverUI();


private:
	EditorGUIManager() {}; 


private:
	static EditorGUIManager* sharedInstance;

	//std::vector<AUITab*> uiList;
	std::unordered_map<std::string, AUITab*> uiTable;
};