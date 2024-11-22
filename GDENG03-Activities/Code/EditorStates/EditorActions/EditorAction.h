#pragma once

#include "../../Vector3D.h"
#include "../../Matrix4x4.h"
#include "../../AGameObject.h"

class EditorAction
{
public:
	EditorAction(AGameObject* obj);
	~EditorAction() = default;

	bool isSimilar(AGameObject* obj);
private:
	std::string id;
	Vector3D m_local_position;
	Vector3D m_local_rotation;
	Vector3D m_local_scale;
	bool isActive;

	friend class EditorActionHistory;
};


