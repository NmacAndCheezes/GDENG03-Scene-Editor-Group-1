#pragma once
#include "./GameEngine/GameObjects/AGameObject.h"
#include "./GameEngine/Components/Transform.h"
#include "./SimpleMath.h"

class EditorAction
{
public:
	EditorAction(AGameObject* obj);
	~EditorAction() = default;

	bool isSimilar(AGameObject* obj);
private:
	unsigned int id;
	Vector3 m_local_position;
	Vector3 m_local_rotation;
	Vector3 m_local_scale;
	bool isEnabled;

	friend class EditorActionHistory;
};


