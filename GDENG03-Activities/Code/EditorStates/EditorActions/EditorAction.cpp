#include "EditorAction.h"
#include "../../AGameObject.h"

EditorAction::EditorAction(AGameObject* obj)
{
	id = obj->getId();
	m_local_position = obj->getTransform()->localPosition();
	m_local_rotation = obj->getTransform()->localRotation();
	m_local_scale = obj->getTransform()->localScale();
	isActive = obj->getActive();
}

bool EditorAction::isSimilar(AGameObject* obj)
{
	Transform* t = obj->getTransform();

	//t->localPosition().printVector("Position 1"); m_local_position.printVector("Position 2");
	return t->localPosition() == m_local_position && 
		t->localRotation() == m_local_rotation && 
		t->localScale() == m_local_scale && 
		obj->getActive() == isActive;
}
