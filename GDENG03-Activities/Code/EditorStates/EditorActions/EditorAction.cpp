#include "EditorAction.h"

EditorAction::EditorAction(AGameObject* obj)
{
	id = obj->GetInstanceID();
	m_local_position = obj->GetTransform()->GetLocalPosition();
	m_local_rotation = obj->GetTransform()->GetLocalEulerAngles();
	m_local_scale = obj->GetTransform()->GetLocalScale();
	isEnabled = obj->IsEnabled();
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
