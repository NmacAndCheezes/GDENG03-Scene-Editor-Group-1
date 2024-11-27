#include "EditorAction.h"
#include <GameEngine/Debug.h>

EditorAction::EditorAction(AGameObject* obj)
{
	id = obj->GetInstanceID();
	name = obj->GetName();
	m_local_position = obj->GetTransform()->GetLocalPosition();
	m_local_rotation = obj->GetTransform()->GetLocalEulerAngles();
	m_local_scale = obj->GetTransform()->GetLocalScale();
	isEnabled = obj->IsEnabled();
	//Debug::Log("Recording Position of " + name + std::to_string(m_local_position.x) + std::to_string(m_local_position.y) + std::to_string(m_local_position.z) + "\n");
	for (auto i : obj->GetAllComponents())
	{
		if (i->GetType() == EComponentTypes::Transform) continue;
		if (i->GetType() == EComponentTypes::Physics)
		{
			RigidBody3D* r = (RigidBody3D*)i;
			lastTransform = r->GetRigidBody()->getTransform();
			
			Vector3 v = Vector3(lastTransform.getPosition().x, lastTransform.getPosition().y, lastTransform.getPosition().z);
			
		}
			
		components.push_back(*i);
	}
}

bool EditorAction::isSimilar(AGameObject* obj)
{
	Transform* t = obj->GetTransform();

	
	//t->localPosition().printVector("Position 1"); m_local_position.printVector("Position 2");
	return obj->GetTransform()->GetPosition() == m_local_position &&
		obj->GetTransform()->GetEulerAngles() == m_local_rotation &&
		obj->GetTransform()->GetLocalScale() == m_local_scale &&
		obj->IsEnabled() == isEnabled;
}
