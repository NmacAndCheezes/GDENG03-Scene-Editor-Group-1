#pragma once
#include "../AComponent.h"
#include "GameEngine/Meshes/EPrimitiveMeshTypes.h"
#include <SimpleMath.h>
#include <reactphysics3d/reactphysics3d.h>


using namespace DirectX::SimpleMath;

class RigidBody3D : public AComponent
{
public:
	RigidBody3D(EPrimitiveMeshTypes shapeType);
	~RigidBody3D();

	void Clone(AComponent* copy) override;
	void Perform() override;
	void RenderUI() override;

	#pragma region Getters-Setters
	bool Init(rp3d::RigidBody* rb);
	rp3d::RigidBody* GetRigidBody();
	EPrimitiveMeshTypes GetMeshType();
	void SetInterpolationFactor(float factor);

	float GetMass();
	void SetMass(const float& newMass);
	__declspec(property(get = GetMass, put = SetMass)) float Mass;

	rp3d::BodyType GetBodyType();
	void SetBodyType(const rp3d::BodyType& newType);
	__declspec(property(get = GetBodyType, put = SetBodyType)) rp3d::BodyType BodyType;

	bool IsGravityEnabled();
	void EnableGravity(const bool& status);
	__declspec(property(get = IsGravityEnabled, put = EnableGravity)) bool GravityEnabled;

	float GetLinearDamping();
	void SetLinearDamping(const float& newDamping);
	__declspec(property(get = GetLinearDamping, put = SetLinearDamping)) float LinearDamping;

	float GetAngularDamping();
	void SetAngularDamping(const float& newDamping);
	__declspec(property(get = GetLinearDamping, put = SetLinearDamping)) float AngularDamping;

	Vector3 GetLinearLocks();
	void SetLinearLocks(const Vector3& newLocks);
	__declspec(property(get = GetLinearLocks, put = SetLinearLocks)) Vector3 LinearLocks;

	Vector3 GetAngularLocks();
	void SetAngularLocks(const Vector3& newLocks);
	__declspec(property(get = GetAngularLocks, put = SetAngularLocks)) Vector3 AngularLocks;
	#pragma endregion

	void UpdateTransform();
	void ApplyForce(const Vector3& force);
	void ApplyForce(const Vector3& force, const Vector3& point);
	void ApplyTorque(const Vector3& torque);


private:
	rp3d::RigidBody* rb;
	rp3d::Transform prevTransform;
	float factor;

	EPrimitiveMeshTypes meshType;
	rp3d::Collider* collider;
	rp3d::Transform meshTransform;
};