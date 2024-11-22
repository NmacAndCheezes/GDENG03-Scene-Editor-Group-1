#include "EditorActionHistory.h"
#include "../../AGameObjectManager.h"
#include "../../Physics/PhysicsSystem.h"
#include "../../ECS Systems/BaseComponentSystem.h"
EditorActionHistory* EditorActionHistory::sharedInstance = nullptr;

void EditorActionHistory::initialize()
{
	if (EditorActionHistory::sharedInstance) throw std::exception("Graphics Engine already created");
	EditorActionHistory::sharedInstance = new EditorActionHistory();
}


void EditorActionHistory::destroy()
{
	if (!EditorActionHistory::sharedInstance) return;
	delete EditorActionHistory::sharedInstance;
}


EditorActionHistory* EditorActionHistory::get()
{
	return sharedInstance;
}

void EditorActionHistory::RecordAction(AGameObject* obj)
{
	if(obj != nullptr)
	UndoActions.push(new EditorAction(obj));
}

void EditorActionHistory::CheckIfSimilar(AGameObject* obj)
{
	EditorAction* gO = UndoActions.top();
	if (gO->isSimilar(obj)) { UndoActions.pop(); return; }

	std::cout << "Undo Actions" << UndoActions.size() << std::endl;
}

void EditorActionHistory::SetToEditState()
{
	for (auto action : EditState)
	{
		AGameObject* obj = AGameObjectManager::get()->getObjectList()[action->id];

		if (obj == nullptr) continue;

		std::cout << "Reverting " << obj->getId() << std::endl;
		obj->getTransform()->SetLocalPosition(action->m_local_position);
		obj->getTransform()->SetLocalRotation(action->m_local_rotation);
		obj->getTransform()->SetLocalScale(action->m_local_scale);
		obj->setActive(action->isActive);
	}
}

void EditorActionHistory::RecordEditStates()
{
	BaseComponentSystem::get()->getPhysicsSystem()->resetAllComponents();
	EditState.clear();
	for (auto i : AGameObjectManager::get()->getObjectList())
	{
		EditorAction* action = new EditorAction((i.second));
		EditState.push_back(action);

		std::cout << "Recording " << i.second->getId() << std::endl;
		action->m_local_position.printVector(i.second->getName());
		action->m_local_rotation.printVector(i.second->getName());
		action->m_local_scale.printVector(i.second->getName());
	}
}

void EditorActionHistory::Undo()
{
	if (UndoActions.size() == 0) { std::cout << "UndoAction is empty" << std::endl; return; }
	EditorAction* action = UndoActions.top();
	
	UndoActions.pop();

	AGameObject* obj = AGameObjectManager::get()->getObjectList()[action->id];
	if (obj == nullptr) return;

	RedoActions.push(new EditorAction(obj));

	obj->getTransform()->SetLocalPosition(action->m_local_position);
	obj->getTransform()->SetLocalRotation(action->m_local_rotation);
	obj->getTransform()->SetLocalScale(action->m_local_scale);
	obj->setActive(action->isActive);
}

void EditorActionHistory::Redo()
{
	if (RedoActions.size() == 0) { std::cout << "RedoAction is empty" << std::endl; return; }
	EditorAction* action = RedoActions.top();
	RedoActions.pop();

	AGameObject* obj = AGameObjectManager::get()->getObjectList()[action->id];
	if (obj == nullptr) return;
	obj->getTransform()->SetLocalPosition(action->m_local_position);
	obj->getTransform()->SetLocalRotation(action->m_local_rotation);
	obj->getTransform()->SetLocalScale(action->m_local_scale);
	obj->setActive(action->isActive);
}

EditorActionHistory::~EditorActionHistory()
{
	UndoActions.empty();
	RedoActions.empty();
	EditState.clear();
}
