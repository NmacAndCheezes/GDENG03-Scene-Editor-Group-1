#include "EditorActionHistory.h"
#include "./GameEngine/Managers/GameObjectManager.h"
#include "./GameEngine/Managers/PhysicsEngine.h"
#include <GameEngine/Debug.h>

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
	if (obj == nullptr) return;

	UndoActions.push(new EditorAction(obj));
	Debug::Log("[EditorActionHistory] Record Action");

}

void EditorActionHistory::CheckIfSimilar(AGameObject* obj)
{
	if (UndoActions.empty()) return;
	EditorAction* gO = UndoActions.top();
	if (gO->isSimilar(obj)) 
	{ 
		UndoActions.pop(); 
		//Debug::Log("[EditorActionHistory] Similar values: " + std::to_string(UndoActions.size()));
	}
}

void EditorActionHistory::SetToEditState()
{
	if (EditState.empty()) return;
	for (auto action : EditState)
	{
		AGameObject* obj = GameObjectManager::GetInstance()->GetGameObjectMap()[action->id];

		if (obj == nullptr) 
		{
			obj = new AGameObject(action->name);
			Debug::Log(action->name);
			GameObjectManager::GetInstance()->AddRootObject(obj);

			for (auto i : action->components)
			{
				Debug::Log(i.GetName());
				obj->AttachComponent(&i);
				obj->Initialize();
				i.Initialize();
			}
		}

		std::cout << "Reverting " << obj->GetInstanceID() << std::endl;

		obj->GetTransform()->SetLocalPosition(action->m_local_position);

		Vector3 diffEuler = action->m_local_rotation - obj->GetTransform()->GetEulerAngles();
		obj->GetTransform()->Rotate(diffEuler);

		obj->GetTransform()->SetLocalScale(action->m_local_scale);
		obj->SetEnabled(action->isEnabled);

		RigidBody3D* rb3D = (RigidBody3D*)obj->FindComponentByName("RigidBody3D");
		if (rb3D != NULL)
		{
			rb3D->GetRigidBody()->setTransform(action->lastTransform);
			rb3D->GetRigidBody()->setAngularVelocity(reactphysics3d::Vector3(0,0,0));
			rb3D->GetRigidBody()->setLinearVelocity(reactphysics3d::Vector3(0, 0, 0));
		}
		
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
	if (UndoActions.empty()) { std::cout << "UndoAction is empty" << std::endl; return; }
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

	Debug::Log("Undoing " + obj->GetName() + " undos left: " + std::to_string(UndoActions.size()));
}

void EditorActionHistory::Redo()
{
	if (RedoActions.empty()) { std::cout << "RedoAction is empty" << std::endl; return; }
	EditorAction* action = RedoActions.top();
	RedoActions.pop();

	AGameObject* obj = GameObjectManager::GetInstance()->GetGameObjectMap()[action->id];
	if (obj == nullptr) return;

	UndoActions.push(new EditorAction(obj));
	obj->GetTransform()->SetLocalPosition(action->m_local_position);

	Vector3 diffEuler = action->m_local_rotation - obj->GetTransform()->GetEulerAngles();
	obj->GetTransform()->Rotate(diffEuler);

	obj->GetTransform()->SetLocalScale(action->m_local_scale);
	obj->SetEnabled(action->isEnabled);
	Debug::Log("Undoing " + obj->GetName());
}

EditorActionHistory::~EditorActionHistory()
{
	EditState.clear();
}
