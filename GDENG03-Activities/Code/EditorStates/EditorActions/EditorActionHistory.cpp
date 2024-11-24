#include "EditorActionHistory.h"
#include "./GameEngine/Managers/GameObjectManager.h"
#include "./GameEngine/Managers/PhysicsEngine.h"

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
	if (EditState.size() == 0) return;
	for (auto action : EditState)
	{
		AGameObject* obj = GameObjectManager::GetInstance()->GetGameObjectMap()[action->id];

		if (obj == nullptr) continue;

		std::cout << "Reverting " << obj->GetInstanceID() << std::endl;
		obj->GetTransform()->SetLocalPosition(action->m_local_position);

		Vector3 diffEuler = action->m_local_rotation - obj->GetTransform()->GetEulerAngles();
		obj->GetTransform()->Rotate(diffEuler);

		obj->GetTransform()->SetLocalScale(action->m_local_scale);
		obj->SetEnabled(action->isEnabled);
	}
}

void EditorActionHistory::RecordEditStates()
{
	//BaseComponentSystem::get()->getPhysicsSystem()->resetAllComponents();
	EditState.clear();
	for (auto i : GameObjectManager::GetInstance()->GetGameObjectMap())
	{
		EditorAction* action = new EditorAction((i.second));
		EditState.push_back(action);

		//std::cout << "Recording " << i.second->getId() << std::endl;
		//action->m_local_position.printVector(i.second->getName());
		//action->m_local_rotation.printVector(i.second->getName());
		//action->m_local_scale.printVector(i.second->getName());
	}
}

void EditorActionHistory::Undo()
{
	if (UndoActions.size() == 0) { std::cout << "UndoAction is empty" << std::endl; return; }
	EditorAction* action = UndoActions.top();
	
	UndoActions.pop();

	AGameObject* obj = GameObjectManager::GetInstance()->GetGameObjectMap()[action->id];
	if (obj == nullptr) return;

	RedoActions.push(new EditorAction(obj));

	obj->GetTransform()->SetLocalPosition(action->m_local_position);

	Vector3 diffEuler = action->m_local_rotation - obj->GetTransform()->GetEulerAngles();
	obj->GetTransform()->Rotate(diffEuler);

	obj->GetTransform()->SetLocalScale(action->m_local_scale);
	obj->SetEnabled(action->isEnabled);
}

void EditorActionHistory::Redo()
{
	if (RedoActions.size() == 0) { std::cout << "RedoAction is empty" << std::endl; return; }
	EditorAction* action = RedoActions.top();
	RedoActions.pop();

	AGameObject* obj = GameObjectManager::GetInstance()->GetGameObjectMap()[action->id];
	if (obj == nullptr) return;
	obj->GetTransform()->SetLocalPosition(action->m_local_position);

	Vector3 diffEuler = action->m_local_rotation - obj->GetTransform()->GetEulerAngles();
	obj->GetTransform()->Rotate(diffEuler);

	obj->GetTransform()->SetLocalScale(action->m_local_scale);
	obj->SetEnabled(action->isEnabled);
}

EditorActionHistory::~EditorActionHistory()
{
	UndoActions.empty();
	RedoActions.empty();
	EditState.clear();
}
