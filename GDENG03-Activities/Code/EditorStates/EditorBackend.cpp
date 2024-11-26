#include "EditorBackend.h"
#include <exception>
#include <GameEngine/Debug.h>

EditorBackend* EditorBackend::sharedInstance = nullptr;

void EditorBackend::setState(EditorState newState)
{
    currentState = newState;
}

EditorBackend::EditorState EditorBackend::getState()
{
    return currentState;
}

void EditorBackend::startFrameStep()
{
	isFramedStep = true;
}

void EditorBackend::endFrameStep()
{
	isFramedStep = false;
}

bool EditorBackend::getIsFrameStep()
{
	return isFramedStep;
}

void EditorBackend::initialize()
{
	if (EditorBackend::sharedInstance) throw std::exception("Graphics Engine already created");
	EditorBackend::sharedInstance = new EditorBackend();
}


void EditorBackend::destroy()
{
	if (!EditorBackend::sharedInstance) return;
	delete EditorBackend::sharedInstance;
}


EditorBackend* EditorBackend::get()
{
	return sharedInstance;
}
