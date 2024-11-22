#pragma once
#include "AGameObject.h"
#include "GameEngine/Components/Physics/Rigidbody3D.h"
#include "GameEngine/Components/Renderer/MeshRenderer.h"


class PhysicsObject : public AGameObject
{
public:
	PhysicsObject(EPrimitiveMeshTypes meshType, float mass = 10.f);
	~PhysicsObject();

	void Initialize() override;
	RigidBody3D* GetRB();


private:
	EPrimitiveMeshTypes meshType;
	float mass;
	RigidBody3D* rb;

	MeshRenderer* renderer;
};