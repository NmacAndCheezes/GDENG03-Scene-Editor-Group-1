#pragma once
#include <stack>
#include "EditorAction.h"
#include "../../AGameObject.h"

class EditorActionHistory
{
public:
	//Initialize the GraphicsEngine and DirectX 11 Device
	static void initialize();
	static void destroy();
	static EditorActionHistory* get();

	void RecordAction(AGameObject* obj);
	void CheckIfSimilar(AGameObject* obj);
	void SetToEditState();
	void RecordEditStates();

	void Undo();
	void Redo();
private:
	EditorActionHistory() = default;
	static EditorActionHistory* sharedInstance;
	EditorActionHistory(EditorActionHistory const&) {}
	EditorActionHistory& operator=(EditorActionHistory const&) {};
	~EditorActionHistory();

private:
	std::list<EditorAction*> EditState;
	std::stack<EditorAction*> UndoActions;
	std::stack<EditorAction*> RedoActions;
};

