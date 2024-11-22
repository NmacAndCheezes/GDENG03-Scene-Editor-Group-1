#include "Transform.h"
#include "../MathUtils.h"
#include "../GameObjects/AGameObject.h"


Transform::Transform() : AComponent("Transform", EComponentTypes::Transform)
{
	globalPos = Vector3::Zero; 
	localPos = Vector3::Zero; 

	globalScale = Vector3::One;  
	localScale = Vector3::One;

	eulerAngles = Vector3::Zero; 
	localEulerAngles = Vector3::Zero;
	orientation = Quaternion::Identity;

	localRight = Vector3::Right;
	localUp = Vector3::Up;
	localForward = Vector3::Backward; // backward returns (0,0,1)
}

Transform::~Transform()
{

}

void Transform::Clone(AComponent* copy)
{
	Transform* copyTransform = (Transform*)copy;
	if (copyTransform == nullptr) return;

	SetPosition(copyTransform->globalPos);

	AGameObject* parent = owner->GetParent();
	Vector3 newScale = (parent) ? 
		MathUtils::CheckDivisionByZero(copyTransform->globalScale, parent->GetTransform()->globalScale) : 
		copyTransform->globalScale;
	SetLocalScale(newScale);

	Vector3 rotation = (parent) ?
		copyTransform->eulerAngles - parent->GetTransform()->eulerAngles - localEulerAngles :
		copyTransform->eulerAngles - eulerAngles;
	Rotate(rotation);
}

void Transform::Perform()
{

}

void Transform::SetEnabled(bool flag)
{

}

void Transform::RenderUI()
{
	float position[3];
	position[0] = localPos.x;
	position[1] = localPos.y;
	position[2] = localPos.z;

	float eulerAngle[3];
	eulerAngle[0] = localEulerAngles.x;
	eulerAngle[1] = localEulerAngles.y;
	eulerAngle[2] = localEulerAngles.z;

	float scale[3];
	scale[0] = localScale.x;
	scale[1] = localScale.y;
	scale[2] = localScale.z;

	ImGui::DragFloat3("Position", position);
	ImGui::DragFloat3("Rotation", eulerAngle);
	ImGui::DragFloat3("Scale", scale);

	Vector3 diffEuler = Vector3(eulerAngle) - localEulerAngles;

	if (Vector3(position) != localPos)           SetLocalPosition(Vector3(position));
	if (Vector3(eulerAngle) != localEulerAngles) Rotate(diffEuler); 
	if (Vector3(scale) != localScale)            SetLocalScale(Vector3(scale)); 
}

TMatrix Transform::GetTransformationMatrix()
{
	return tMatrix;
}

void Transform::UpdateTransformationMatrix()
{
	tMatrix = TMatrix{
			Matrix::CreateScale(globalScale) * 
			Matrix::CreateFromQuaternion(orientation) * 
			Matrix::CreateTranslation(globalPos) 
	};
}

void Transform::RecalculateChildTransformWithoutParent() 
{
	localPos = globalPos;
	localScale = globalScale;
	localEulerAngles = eulerAngles;

	// no need to update children since the local and global values are still the same 
}

void Transform::RecalculateChildTransformWithParent(const Transform* parent)
{
	SetLocalPosition(globalPos - parent->globalPos);
	SetLocalScale(MathUtils::CheckDivisionByZero(globalScale, parent->globalScale));
	UpdateLocalEulerAnglesWithChildren(parent);
}


#pragma region Position
Vector3 Transform::GetPosition()
{
	return globalPos;
}

void Transform::SetPosition(const Vector3& newPos)
{
	// set value
	globalPos = newPos;

	// update local position, taking consideration of the parent
	AGameObject* parentObj = owner->GetParent();

	localPos = (parentObj) ? 
		globalPos - parentObj->GetTransform()->globalPos : 
		globalPos; 

	// update all the transforms of every 'descendant'
	auto transformsFromChildren = owner->GetComponentsInChildrenOfType(EComponentTypes::Transform);
	for (int i = 0; i < transformsFromChildren.size(); i++)
	{
		Transform* childTransform = (Transform*)transformsFromChildren[i];
		childTransform->SetPosition(newPos + childTransform->localPos);
	}

	UpdateTransformationMatrix();
}

Vector3 Transform::GetLocalPosition()
{
	return localPos;
}

void Transform::SetLocalPosition(const Vector3& newPos)
{
	// set value
	localPos = newPos;

	// update global position, taking consideration of the parent
	AGameObject* parentObj = owner->GetParent(); 
	globalPos = (parentObj) ?
		localPos + parentObj->GetTransform()->globalPos :
		localPos;

	// update all the transforms of every 'descendant'
	auto transformsFromChildren = owner->GetComponentsInChildrenOfType(EComponentTypes::Transform); 
	for (int i = 0; i < transformsFromChildren.size(); i++) 
	{
		Transform* childTransform = (Transform*)transformsFromChildren[i]; 
		childTransform->SetPosition(globalPos + childTransform->localPos); 
	}

	UpdateTransformationMatrix();
}
#pragma endregion


#pragma region Scale
Vector3 Transform::GetLocalScale()
{
	return localScale;
}

void Transform::SetLocalScale(const Vector3& newScale)
{
	// set value
	localScale = newScale; 

	// update global scale
	UpdateGlobalScaleWithChildren(); 
}

void Transform::ScaleUniformly(float factor) 
{
	localScale *= factor; 

	UpdateGlobalScaleWithChildren(); 
}

void Transform::UpdateGlobalScaleWithChildren()
{
	// update global scale, taking consideration of the parent
	AGameObject* parentObj = owner->GetParent();
	globalScale = (parentObj) ?
		localScale * parentObj->GetTransform()->globalScale :
		localScale;

	// update all the transforms of every 'descendant'
	auto transformsFromChildren = owner->GetComponentsInChildrenOfType(EComponentTypes::Transform);
	for (int i = 0; i < transformsFromChildren.size(); i++)
	{
		Transform* childTransform = (Transform*)transformsFromChildren[i];
		childTransform->UpdateGlobalScaleWithChildren();
	}

	UpdateTransformationMatrix(); 
}
#pragma endregion 


#pragma region Rotation
Quaternion Transform::GetOrientation()
{
	return orientation;
}
Vector3 Transform::GetEulerAngles()
{
	return eulerAngles;
}

Vector3 Transform::GetLocalEulerAngles()
{
	return localEulerAngles;
}

Vector3 Transform::GetLocalRight() 
{
	return localRight;
}

Vector3 Transform::GetLocalUp() 
{
	return localUp;
}

Vector3 Transform::GetLocalForward() 
{
	return localForward;
}

void Transform::Rotate(float xAngleInDeg, float yAngleInDeg, float zAngleInDeg)
{
	Rotate(Vector3(xAngleInDeg, yAngleInDeg, zAngleInDeg));
}

void Transform::Rotate(const Vector3& eulerInDeg)
{
	// setup axis vectors when rotating the object on each axis
	Vector3 axisUp = (owner->GetParent()) ?
		owner->GetParent()->GetTransform()->localUp :
		Vector3::Up;

	Vector3 refRight = (owner->GetParent()) ? owner->GetParent()->GetTransform()->localRight : Vector3::Right;
	Quaternion rotateRight = Quaternion::CreateFromAxisAngle((owner->GetParent()) ?
		owner->GetParent()->GetTransform()->localUp : Vector3::Up, 
		eulerAngles.y * MathUtils::Deg2Rad); 
	Vector3 axisRight = Vector3::Transform(refRight, rotateRight);

	// combine all rotations using the axis vectors, then apply to current orientation
	Quaternion yaw = Quaternion::CreateFromAxisAngle(axisUp, eulerInDeg.y * MathUtils::Deg2Rad);  
	Quaternion pitch = Quaternion::CreateFromAxisAngle(axisRight, eulerInDeg.x * MathUtils::Deg2Rad);  
	Quaternion roll = Quaternion::CreateFromAxisAngle(localForward, eulerInDeg.z * MathUtils::Deg2Rad);
	Quaternion toRotate = roll * pitch * yaw; 
	orientation *= toRotate;

	// update euler angles and local vectors
	eulerAngles += eulerInDeg; 
	eulerAngles = MathUtils::GetNearestReferenceAngles(eulerAngles);
	UpdateLocalVectors(); 

	// update local euler angles, taking consideration of the parent
	AGameObject* parentObj = owner->GetParent();

	localEulerAngles = (parentObj) ?
		eulerAngles - parentObj->GetTransform()->eulerAngles :
		eulerAngles;
	localEulerAngles = MathUtils::GetNearestReferenceAngles(localEulerAngles); 

	// update all the transforms of every 'descendant'
	auto transformsFromChildren = owner->GetComponentsInChildrenOfType(EComponentTypes::Transform);
	for (int i = 0; i < transformsFromChildren.size(); i++)
	{
		Transform* childTransform = (Transform*)transformsFromChildren[i];
		childTransform->RotateFromParent(eulerInDeg, toRotate, this);
	}

	UpdateTransformationMatrix();
}

void Transform::Rotate(const Quaternion& toRotate)
{
	orientation *= toRotate;

	// update euler angles and local vectors
	eulerAngles += toRotate.ToEuler() * MathUtils::Rad2Deg; 
	eulerAngles = MathUtils::GetNearestReferenceAngles(eulerAngles);
	UpdateLocalVectors();

	// update local euler angles, taking consideration of the parent
	AGameObject* parentObj = owner->GetParent();

	localEulerAngles = (parentObj) ?
		eulerAngles - parentObj->GetTransform()->eulerAngles :
		eulerAngles;
	localEulerAngles = MathUtils::GetNearestReferenceAngles(localEulerAngles);

	// update all the transforms of every 'descendant'
	auto transformsFromChildren = owner->GetComponentsInChildrenOfType(EComponentTypes::Transform);
	for (int i = 0; i < transformsFromChildren.size(); i++)
	{
		Transform* childTransform = (Transform*)transformsFromChildren[i];
		childTransform->RotateFromParent(toRotate.ToEuler() * MathUtils::Rad2Deg, toRotate, this);
	}

	UpdateTransformationMatrix();
}

void Transform::RotateFromParent(const Vector3& eulerInDeg, const Quaternion& toRotate, const Transform* parent) // revisit parent rotations, not yet accurate
{
	orientation *= toRotate;  
	 
	// update all euler angles and local vectors
	eulerAngles += eulerInDeg;
	eulerAngles = MathUtils::GetNearestReferenceAngles(eulerAngles); 
	localEulerAngles = eulerAngles - parent->eulerAngles; 
	localEulerAngles = MathUtils::GetNearestReferenceAngles(localEulerAngles); 
	UpdateLocalVectors(); 

	// update position based on an offset from parent
	Vector3 displacement = globalPos - parent->globalPos;
	displacement = Vector3::Transform(displacement, toRotate);
	SetPosition(parent->globalPos + displacement);

	// update all the transforms of every 'descendant'
	auto transformsFromChildren = owner->GetComponentsInChildrenOfType(EComponentTypes::Transform); 
	for (int i = 0; i < transformsFromChildren.size(); i++) 
	{
		Transform* childTransform = (Transform*)transformsFromChildren[i]; 
		childTransform->RotateFromParent(eulerInDeg, toRotate, this);
	}

	UpdateTransformationMatrix();
}


//void Transform::Rotate(const Vector3& axis, float angleInDeg)
//{
//	Quaternion toRotate = Quaternion::CreateFromAxisAngle(axis, DirectX::XMConvertToRadians(angleInDeg)); 
//	orientation *= toRotate;
//
//	// update euler angles and local vectors
//	eulerAngles += toRotate.ToEuler() * (180.f / M_PI);
//	UpdateLocalVectors();
//
//	// update local euler angles, taking consideration of the parent
//	AGameObject* parentObj = owner->GetParent(); 
//
//	localEulerAngles = (parentObj) ? 
//		eulerAngles - parentObj->GetTransform()->eulerAngles : 
//		eulerAngles;
//
//	// update all the transforms of every 'descendant'
//	auto transformsFromChildren = owner->GetComponentsInChildrenOfType(EComponentTypes::Transform); 
//	for (int i = 0; i < transformsFromChildren.size(); i++) 
//	{
//		Transform* childTransform = (Transform*)transformsFromChildren[i]; 
//		childTransform->RotateFromParent(toRotate, this);
//	}
//
//	UpdateTransformationMatrix();
//}
//
//void Transform::RotateFromParent(const Quaternion& toRotate, const Transform* parent)
//{
//	orientation *= toRotate;
//
//	// update all euler angles and local vectors
//	eulerAngles += toRotate.ToEuler() * (180.f / M_PI);
//	localEulerAngles = eulerAngles - parent->eulerAngles;
//	UpdateLocalVectors();
//
//	// update position based on an offset from parent
//	Vector3 displacement = globalPos - parent->globalPos; 
//	displacement = Vector3::Transform(displacement, toRotate);
//	SetPosition(parent->globalPos + displacement); 
//
//	// update all the transforms of every 'descendant'
//	auto transformsFromChildren = owner->GetComponentsInChildrenOfType(EComponentTypes::Transform);
//	for (int i = 0; i < transformsFromChildren.size(); i++)
//	{
//		Transform* childTransform = (Transform*)transformsFromChildren[i];
//		childTransform->RotateFromParent(toRotate, this);
//	}
//
//	UpdateTransformationMatrix();
//}


void Transform::UpdateLocalEulerAnglesWithChildren(const Transform* parent)
{
	localEulerAngles = eulerAngles - parent->eulerAngles;

	auto transformsFromChildren = owner->GetComponentsInChildrenOfType(EComponentTypes::Transform); 
	for (int i = 0; i < transformsFromChildren.size(); i++) 
	{
		Transform* childTransform = (Transform*)transformsFromChildren[i]; 
		childTransform->UpdateLocalEulerAnglesWithChildren(this);
	}
}

void Transform::UpdateLocalVectors()
{
	Vector3::Transform(Vector3::Right, orientation, localRight);
	Vector3::Transform(Vector3::Up, orientation, localUp);
	Vector3::Transform(Vector3::Backward, orientation, localForward); // backward returns (0,0,1)
}
#pragma endregion