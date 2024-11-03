#pragma once
#include "AUIScreen.h"
#include "../WindowSystem/Window.h"
#include <map>
#include <string>

class EditorUIManager
{
public:
	static void initialize(HWND hwnd);
	void update();
	static void destroy();
	static EditorUIManager* get();

	AUIScreen* getUIScreen(std::string name);
private:
	EditorUIManager(HWND hwnd);
	static EditorUIManager* sharedInstance;
	EditorUIManager(EditorUIManager const&) {}
	EditorUIManager& operator=(EditorUIManager const&) {};
	~EditorUIManager();

	std::map<std::string, AUIScreen*> m_UIScreenList;
};

