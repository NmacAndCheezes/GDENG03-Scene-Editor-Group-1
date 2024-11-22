#include "PhysicsEngine.h"
#include "../MathUtils.h"
#include "MeshManager.h"
#include "GameEngine/Components/Transform.h"
#include "GameEngine/GameObjects/AGameObject.h"


#pragma region Singleton
PhysicsEngine* PhysicsEngine::sharedInstance = NULL;

PhysicsEngine* PhysicsEngine::GetInstance()
{
	if (sharedInstance == NULL)
	{
		sharedInstance = new PhysicsEngine();
	}

	return sharedInstance;
}
#pragma endregion


bool PhysicsEngine::Init()
{
	physicsCommon = new rp3d::PhysicsCommon();

	rp3d::PhysicsWorld::WorldSettings settings;
	settings.defaultVelocitySolverNbIterations = 150; 
	settings.defaultPositionSolverNbIterations = 50;
	settings.isSleepingEnabled = true;
	settings.gravity = rp3d::Vector3(0, -9.81, 0);  
	settings.restitutionVelocityThreshold = 0.25f;
	settings.defaultFrictionCoefficient = 0.7f;

	physicsWorld = physicsCommon->createPhysicsWorld(settings);
	physicsWorld->setTimeBeforeSleep(0.03f);

	return true;
}

bool PhysicsEngine::Release()
{
	for (int i = (int)rigidBodyList.size() - 1; i >= 0; i--)
	{
		UnregisterRigidBody(rigidBodyList[i]);
	}
	rigidBodyList.clear();
	rigidBodyList.shrink_to_fit();

	for (auto& shapePair : primitiveShapesTable)
	{
		DestroyPrimitiveShape(shapePair.second);
		shapePair.second = nullptr; 
		//primitiveShapesTable.erase(shapePair.first); 
	}
	primitiveShapesTable.clear();

	for (auto& meshPair : convexMeshTable)
	{
		physicsCommon->destroyConvexMesh(meshPair.second);
		convexMeshTable[meshPair.first] = nullptr; 
		//convexMeshTable.erase(meshPair.first); 
	}
	convexMeshTable.clear();

	physicsCommon->destroyPhysicsWorld(physicsWorld);
	delete physicsCommon;

	return true;
}

void PhysicsEngine::UpdateWorld(float dt)
{
	for (auto& rb : rigidBodyList)
	{
		rb->EnablePhysics(rb->Enabled && rb->GetOwner()->Enabled);
	}

	physicsWorld->update(dt);
}

void PhysicsEngine::UpdateRigidBodies(float factor)
{ 
	for (auto& rb : rigidBodyList) 
	{
		if (rb->Enabled && rb->GetOwner()->Enabled)
		{
			rb->SetInterpolationFactor(factor); 
			rb->Perform(); 
		}
	}
}

rp3d::PhysicsCommon* PhysicsEngine::GetPhysicsCommon() 
{
	return physicsCommon; 
}


void PhysicsEngine::RegisterRigidBody(RigidBody3D* rb)
{
	auto itr = std::find(rigidBodyList.begin(), rigidBodyList.end(), rb);
	if (itr != rigidBodyList.end()) return;

	rp3d::RigidBody* newRB = CreateRigidBody(rb->GetTransform());
	if (rb->Init(newRB)) rigidBodyList.push_back(rb);
	else physicsWorld->destroyRigidBody(newRB);
}

void PhysicsEngine::UnregisterRigidBody(RigidBody3D* rb)
{
	auto itr = std::find(rigidBodyList.begin(), rigidBodyList.end(), rb);
	if (itr == rigidBodyList.end())  return;
	
	physicsWorld->destroyRigidBody(rb->GetRigidBody());

	rigidBodyList.erase(itr); 
	rigidBodyList.shrink_to_fit(); 
}

rp3d::RigidBody* PhysicsEngine::CreateRigidBody(Transform* transform)
{
	rp3d::Vector3 position = MathUtils::ConvertVector(transform->Position);
	rp3d::Quaternion orientation = MathUtils::ConvertQuaternion(transform->GetOrientation());
	rp3d::Transform rTransform = rp3d::Transform(position, orientation);

	return physicsWorld->createRigidBody(rTransform);
}

void PhysicsEngine::DestroyRigidBody(rp3d::RigidBody* rb)
{
	if (rb == nullptr) return;
	physicsWorld->destroyRigidBody(rb);
}

rp3d::CollisionShape* PhysicsEngine::CreatePrimitiveShape(EPrimitiveMeshTypes meshType, Vector3 scaling, unsigned int instanceID)
{
	// destroy the old shape
	if (primitiveShapesTable.find(instanceID) != primitiveShapesTable.end())
	{
		DestroyPrimitiveShape(primitiveShapesTable[instanceID]);
	}

	// check if mesh has predefined shape
	switch (meshType)
	{
		case EPrimitiveMeshTypes::Cube:
		{ 
			primitiveShapesTable[instanceID] = physicsCommon->createBoxShape(
				rp3d::Vector3(0.5f * scaling.x, 0.5f * scaling.y, 0.5f * scaling.z)
			);
			return primitiveShapesTable[instanceID]; 
		}
		case EPrimitiveMeshTypes::Plane:
		{
			primitiveShapesTable[instanceID] = physicsCommon->createBoxShape( 
				rp3d::Vector3(2.5f * scaling.x, 0.1f * scaling.y, 2.5f * scaling.z)
			);
			return primitiveShapesTable[instanceID]; 
		}
		case EPrimitiveMeshTypes::Sphere:
		{ 
			float biggerScale = scaling.x;
			if (scaling.y > biggerScale) biggerScale = scaling.y; 
			if (scaling.z > biggerScale) biggerScale = scaling.z;
			primitiveShapesTable[instanceID] = physicsCommon->createSphereShape(1.0f * biggerScale);
			return primitiveShapesTable[instanceID]; 
		}
		default: break;
	}

	// check if convex shape is already created
	rp3d::ConvexMesh* mesh = convexMeshTable[meshType];
	if (mesh)
	{
		rp3d::ConvexMeshShape* convexMeshShape = physicsCommon->createConvexMeshShape(mesh, MathUtils::ConvertVector(scaling));
		primitiveShapesTable[instanceID] = convexMeshShape; 
		return convexMeshShape;
	}

	// convert vertices into array of floats
	auto vertices = MeshManager::GetInstance()->GetVertexDataFromMesh(meshType);
	if (vertices.size() == 0) return nullptr;

	float* vertexPosOnly = new float[vertices.size() * 3];
	for (int i = 0; i < vertices.size(); i++)
	{
		vertexPosOnly[i * 3] = vertices[i].pos.x;
		vertexPosOnly[i * 3 + 1] = vertices[i].pos.y;
		vertexPosOnly[i * 3 + 2] = vertices[i].pos.z;
	}

	//// invert indices in counter-clockwise order
	//auto indices = (meshType != EPrimitiveMeshTypes::Unknown) ? 
	//	MeshManager::GetInstance()->GetIndexDataFromMesh(meshType) : 
	//	MeshManager::GetInstance()->GetIndexDataFromMesh(meshName); 

	//int* invertedIndices = new int[indices.size()];
	//for (int i = 0; i < indices.size(); i+=3)
	//{
	//	invertedIndices[i] = indices[i + 2];
	//	invertedIndices[i + 1] = indices[i + 1];
	//	invertedIndices[i + 2] = indices[i];
	//}

	//// create faces of the convex mesh
	//int numFaces = indices.size() / 3;
	//rp3d::PolygonVertexArray::PolygonFace* polygonFaces = new rp3d::PolygonVertexArray::PolygonFace[numFaces];
	//rp3d::PolygonVertexArray::PolygonFace* face = polygonFaces; 
	//for (int f = 0; f < numFaces; f++) 
	//{
	//	// First vertex of the face in the indices array 
	//	face->indexBase = f * 3; 
	//	// Number of vertices in the face
	//	face->nbVertices = 3; 

	//	face++;
	//}

	//// create the polygon vertex array
	//rp3d::PolygonVertexArray polygonVertexArray = rp3d::PolygonVertexArray(vertices.size(), vertexPosOnly, 3 * sizeof(float),
	//	invertedIndices, sizeof(int), numFaces, polygonFaces,
	//	rp3d::PolygonVertexArray::VertexDataType::VERTEX_FLOAT_TYPE, 
	//	rp3d::PolygonVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

	// create convex shape	
	rp3d::VertexArray vertexArray(vertexPosOnly, 3 * sizeof(float), vertices.size(), rp3d::VertexArray::DataType::VERTEX_FLOAT_TYPE);
	std::vector<rp3d::Message> messages;
	mesh = physicsCommon->createConvexMesh(vertexArray, messages);

	//std::vector<rp3d::Message> messages;
	//rp3d::ConvexMesh* convexMesh = physicsCommon->createConvexMesh(polygonVertexArray, messages);

	// cleanup
	delete[] vertexPosOnly;
	//delete[] invertedIndices; 

	// display the messages (info, warning and errors)
	if (messages.size() > 0) {

		for (const rp3d::Message& message : messages) {

			std::string messageType;
			switch (message.type) {
			case rp3d::Message::Type::Information:
				messageType = "info";
				break;
			case rp3d::Message::Type::Warning:
				messageType = "warning";
				break;
			case rp3d::Message::Type::Error:
				messageType = "error";
				break;
			}

			OutputDebugString(("Message (" + messageType + "): " + message.text).c_str());
		}
	}

	if (mesh == nullptr) return nullptr;

	rp3d::ConvexMeshShape* convexMeshShape = physicsCommon->createConvexMeshShape(mesh, MathUtils::ConvertVector(scaling));
	primitiveShapesTable[instanceID] = convexMeshShape;
	convexMeshTable[meshType] = mesh;

	return convexMeshShape; 
}

void PhysicsEngine::DestroyPrimitiveShape(rp3d::CollisionShape* shape)
{
	rp3d::BoxShape* box = (rp3d::BoxShape*)shape;
	if (box) { physicsCommon->destroyBoxShape(box); return; }

	rp3d::SphereShape* sphere = (rp3d::SphereShape*)shape;
	if (sphere) { physicsCommon->destroySphereShape(sphere); return; }

	rp3d::ConvexMeshShape* convex = (rp3d::ConvexMeshShape*)shape;
	if (convex) { physicsCommon->destroyConvexMeshShape(convex); return; }
}