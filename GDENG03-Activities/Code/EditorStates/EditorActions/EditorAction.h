#pragma once
#include "./GameEngine/GameObjects/AGameObject.h"
#include "./GameEngine/Components/Transform.h"
#include "./SimpleMath.h"
#include <reactphysics3d/reactphysics3d.h>
#include "./GameEngine/Components/Physics/Rigidbody3D.h"

class EditorAction
{
public:
	EditorAction(AGameObject* obj);
	~EditorAction() = default;

	bool isSimilar(AGameObject* obj);
private:
	unsigned int id;
	std::string name;
	Vector3 m_local_position;
	Vector3 m_local_rotation;
	Vector3 m_local_scale;

	reactphysics3d::Transform lastTransform;
	bool isEnabled;

	std::vector<AComponent> components;

	friend class EditorActionHistory;
};


