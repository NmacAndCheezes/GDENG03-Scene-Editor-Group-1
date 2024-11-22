#pragma once
#include "GameEngine/Meshes/EPrimitiveMeshTypes.h"
#include "GameEngine/Components/Physics/Rigidbody3D.h"
#include <reactphysics3d/reactphysics3d.h>


class PhysicsEngine
{
public:
	static PhysicsEngine* GetInstance();
	PhysicsEngine(const PhysicsEngine&) = delete;
	PhysicsEngine& operator=(const PhysicsEngine&) = delete;

	bool Init();
	bool Release();
	void UpdateWorld(float dt);
	void UpdateRigidBodies(float factor);

	rp3d::PhysicsCommon* GetPhysicsCommon();

	void RegisterRigidBody(RigidBody3D* rb);
	void UnregisterRigidBody(RigidBody3D* rb);
	rp3d::CollisionShape* CreatePrimitiveShape(EPrimitiveMeshTypes meshType, Vector3 scaling, unsigned int instanceID);

private:
	PhysicsEngine() {};
	~PhysicsEngine() {};

	void DestroyPrimitiveShape(rp3d::CollisionShape* shape);


private:
	static PhysicsEngine* sharedInstance;

	rp3d::PhysicsCommon* physicsCommon;
	rp3d::PhysicsWorld* physicsWorld;

	std::vector<RigidBody3D*> rigidBodyList;

	std::unordered_map<unsigned int, rp3d::CollisionShape*> primitiveShapesTable;
	std::unordered_map< EPrimitiveMeshTypes, rp3d::ConvexMesh*> convexMeshTable;



	std::unordered_map<EPrimitiveMeshTypes, rp3d::CollisionShape*> temp;
	std::vector<rp3d::ConvexMesh*> convexMeshList; 
};