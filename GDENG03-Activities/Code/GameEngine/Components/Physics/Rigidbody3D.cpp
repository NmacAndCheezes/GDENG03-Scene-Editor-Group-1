#include "Rigidbody3D.h"
#include "../Transform.h"
#include "GameEngine/MathUtils.h"
#include "GameEngine/Managers/PhysicsEngine.h"
#include "GameEngine/GameObjects/AGameObject.h"

RigidBody3D::RigidBody3D(EPrimitiveMeshTypes shapeType) : AComponent("RigidBody3D", EComponentTypes::Physics), meshType(shapeType)
{
	rb = nullptr;
	collider = nullptr;
	meshTransform = rp3d::Transform::identity();
}

RigidBody3D::~RigidBody3D() 
{

}

void RigidBody3D::Clone(AComponent* copy)
{
	RigidBody3D* copyRB = (RigidBody3D*)copy;
	if (copyRB == nullptr) return;

	currBodyType = copyRB->currBodyType;
	prevTransform = copyRB->prevTransform;

	// add mesh cloning 

	Mass = copyRB->Mass;
	BodyType = copyRB->BodyType;
	GravityEnabled = copyRB->GravityEnabled;
	LinearDamping = copyRB->LinearDamping;
	AngularDamping = copyRB->AngularDamping;
	LinearLocks = copyRB->LinearLocks;
	AngularLocks = copyRB->AngularLocks;
}

void RigidBody3D::Perform()
{
	if (rb == nullptr) return;

	rp3d::Transform lerpTransform = rp3d::Transform::interpolateTransforms(prevTransform, rb->getTransform(), factor);
	prevTransform = rb->getTransform();

	transform->Position = MathUtils::ConvertVector(lerpTransform.getPosition());

	Quaternion q = transform->GetOrientation();

	Quaternion lerpQuat = MathUtils::ConvertQuaternion(lerpTransform.getOrientation()); 
	Quaternion inverseQuat = transform->GetOrientation();  
	inverseQuat.Inverse(inverseQuat);  

	Quaternion diffQuat = inverseQuat * lerpQuat;
	transform->Rotate(diffQuat);
}

void RigidBody3D::RenderUI()
{
	// create temporary vars
	float mass = Mass;
	const char* body_types[] = { "STATIC", "KINEMATIC", "DYNAMIC" }; int currentBody = (int)currBodyType;
	bool gravityEnabled = GravityEnabled;
	float linearDamping = LinearDamping;
	float angularDamping = AngularDamping;

	const std::string axis[] = { "x", "y", "z" };
	bool linearLocks[] = { !LinearLocks.x, !LinearLocks.y, !LinearLocks.z };
	bool angularLocks[] = { !AngularLocks.x, !AngularLocks.y, !AngularLocks.z };

	// implement UI
	ImGui::DragFloat("Mass", &mass, 1.0f, 0.0f);
	ImGui::Combo("Body Type", &currentBody, body_types, IM_ARRAYSIZE(body_types));
	ImGui::Checkbox("Enable Gravity", &gravityEnabled);
	ImGui::DragFloat("Linear Damping", &linearDamping, 1.0f, 0.0f);
	ImGui::DragFloat("Angular Damping", &angularDamping, 1.0f, 0.f);
	
	/*ImGui::Text("Freeze Position"); 
	ImGui::Dummy(ImVec2(10.f, 0.f));
	for (int i = 0; i < 3; i++)
	{
		ImGui::SameLine();
		ImGui::Checkbox((axis[i] + "##pos").c_str(), &linearLocks[i]);
	}

	ImGui::Text("Freeze Rotation");
	ImGui::Dummy(ImVec2(10.f, 0.f));
	for (int i = 0; i < 3; i++)
	{
		ImGui::SameLine();
		ImGui::Checkbox((axis[i] + "##rot").c_str(), &angularLocks[i]);
	}*/

	// update values
	if (mass != Mass)                            Mass = mass;
	if ((rp3d::BodyType)currentBody != BodyType) BodyType = (rp3d::BodyType)currentBody; 
	if (gravityEnabled != GravityEnabled)        GravityEnabled = gravityEnabled;
	if (linearDamping != LinearDamping)          LinearDamping = linearDamping;
	if (angularDamping != AngularDamping)        AngularDamping = angularDamping;
	if (linearLocks[0] == LinearLocks.x || 
		linearLocks[1] == LinearLocks.y || 
		linearLocks[2] == LinearLocks.z)         LinearLocks = Vector3(!linearLocks[0], !linearLocks[1], !linearLocks[2]); 
	if (angularLocks[0] == AngularLocks.x ||
		angularLocks[1] == AngularLocks.y ||
		angularLocks[2] == AngularLocks.z)       AngularLocks = Vector3(!angularLocks[0], !angularLocks[1], !angularLocks[2]); 

}

#pragma region Getters-Setters
bool RigidBody3D::Init(rp3d::RigidBody* rb)
{
	if (rb == nullptr) return false;

	this->rb = rb;

	colliderShape = PhysicsEngine::GetInstance()->CreatePrimitiveShape(meshType, transform->LocalScale, owner->GetInstanceID());

	if (colliderShape == nullptr) return false;

	meshTransform = rp3d::Transform::identity();   
	collider = rb->addCollider(colliderShape, meshTransform);
	collider->setIsTrigger(false);
	//rb->updateMassPropertiesFromColliders();

	mass = rb->getMass();
	currBodyType = rb->getType();
	isGravityEnabled = rb->isGravityEnabled();
	linearDamping = rb->getLinearDamping();
	angularDamping = rb->getAngularDamping();
	linearLocks = MathUtils::ConvertVector(rb->getLinearLockAxisFactor());
	angularLocks = MathUtils::ConvertVector(rb->getAngularLockAxisFactor());
	prevTransform = rb->getTransform();
	isPhysicsEnabled = true;
	
	return true;
}

rp3d::RigidBody* RigidBody3D::GetRigidBody()
{
	return rb;
}

EPrimitiveMeshTypes RigidBody3D::GetMeshType()
{
	return meshType;
}

void RigidBody3D::SetInterpolationFactor(float factor)
{
	this->factor = factor;
}

float RigidBody3D::GetMass()
{
	return mass;
}

void RigidBody3D::SetMass(const float& newMass)
{
	mass = newMass;
	if (rb) rb->setMass(newMass);
	//rb->updateMassPropertiesFromColliders();
}

rp3d::BodyType RigidBody3D::GetBodyType()
{
	return currBodyType;
}

void RigidBody3D::SetBodyType(const rp3d::BodyType& newType)
{
	currBodyType = newType;
	if (rb) rb->setType(newType);
}

bool RigidBody3D::IsGravityEnabled()
{
	return isGravityEnabled;
}

void RigidBody3D::EnableGravity(const bool& status)
{
	isGravityEnabled = status;
	if (rb)
	{
		rb->enableGravity(status);
		rb->setType(rp3d::BodyType::STATIC); // considered a HACK, either that or the lib is not consistent
		rb->setType(currBodyType);
	}
}

float RigidBody3D::GetLinearDamping()
{
	return linearDamping;
}

void RigidBody3D::SetLinearDamping(const float& newDamping)
{
	linearDamping = newDamping;
	if (rb) rb->setLinearDamping(newDamping);
}

float RigidBody3D::GetAngularDamping()
{
	return angularDamping;
}

void RigidBody3D::SetAngularDamping(const float& newDamping)
{
	angularDamping = newDamping;
	if (rb) rb->setAngularDamping(newDamping);
}

Vector3 RigidBody3D::GetLinearLocks()
{
	return linearLocks;
}

void RigidBody3D::SetLinearLocks(const Vector3& newLocks)
{
	linearLocks = newLocks;
	if (rb) rb->setLinearLockAxisFactor(MathUtils::ConvertVector(newLocks));
}

Vector3 RigidBody3D::GetAngularLocks()
{
	return angularLocks;
}

void RigidBody3D::SetAngularLocks(const Vector3& newLocks)
{
	angularLocks = newLocks;
	if (rb) rb->setAngularLockAxisFactor(MathUtils::ConvertVector(newLocks));
}
#pragma endregion


void RigidBody3D::UpdateTransform()
{
	if (rb == nullptr) return;

	rp3d::Transform newTransform = rp3d::Transform(MathUtils::ConvertVector(transform->Position), MathUtils::ConvertQuaternion(transform->GetOrientation()));
	rb->setTransform(newTransform);

	prevTransform = newTransform;
}

void RigidBody3D::EnablePhysics(bool flag)
{
	if (isPhysicsEnabled == flag) return;

	isPhysicsEnabled = flag;
	if (flag) 
	{
		rb = PhysicsEngine::GetInstance()->CreateRigidBody(transform); 

		rb->setMass(mass);
		rb->setType(currBodyType);
		rb->enableGravity(isGravityEnabled);
		rb->setLinearDamping(linearDamping);
		rb->setAngularDamping(angularDamping);
		rb->setLinearLockAxisFactor(MathUtils::ConvertVector(linearLocks));
		rb->setAngularLockAxisFactor(MathUtils::ConvertVector(angularLocks));
		rb->setLinearVelocity(currLinearVelocity); 
		rb->setAngularVelocity(currAngularVelocity);

		meshTransform = rp3d::Transform::identity(); 
		collider = rb->addCollider(colliderShape, meshTransform); 
	}
	else
	{
		currLinearVelocity = rb->getLinearVelocity();
		currAngularVelocity = rb->getAngularVelocity();
		PhysicsEngine::GetInstance()->DestroyRigidBody(rb);
		rb = nullptr;
		collider = nullptr;
	}
}

void RigidBody3D::ApplyForce(const Vector3& force)
{
	rb->applyLocalForceAtCenterOfMass(MathUtils::ConvertVector(force));
}

void RigidBody3D::ApplyForce(const Vector3& force, const Vector3& point)
{
	rb->applyLocalForceAtLocalPosition(MathUtils::ConvertVector(force), MathUtils::ConvertVector(point));
}

void RigidBody3D::ApplyTorque(const Vector3& torque)
{
	rb->applyLocalTorque(MathUtils::ConvertVector(torque));
}