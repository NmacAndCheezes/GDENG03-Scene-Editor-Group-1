#include "PhysicsObject.h"
#include "GameEngine/Managers/PhysicsEngine.h"
#include "GameEngine/Managers/GameObjectManager.h"


PhysicsObject::PhysicsObject(EPrimitiveMeshTypes meshType, float mass) : AGameObject("PhysicsObject"), meshType(meshType), mass(mass)
{
	rb = nullptr;
	renderer = nullptr;
}

PhysicsObject::~PhysicsObject()
{
	if (rb) PhysicsEngine::GetInstance()->UnregisterRigidBody(rb);
	if (renderer) renderer->Release();
}

void PhysicsObject::Initialize()
{
	rb = new RigidBody3D(meshType);
	AttachComponent(rb);
	PhysicsEngine::GetInstance()->RegisterRigidBody(rb);
	rb->UpdateTransform();
	rb->Mass = mass;

	renderer = new MeshRenderer();
	renderer->LoadPrimitive(meshType, true);
	AttachComponent(renderer); 
	GameObjectManager::GetInstance()->BindRendererToShader(renderer); 

	isInitialized = true;
}

RigidBody3D* PhysicsObject::GetRB() 
{
	return rb;
}