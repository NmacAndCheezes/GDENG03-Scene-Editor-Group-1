#pragma once
#include <SimpleMath.h>
#include "AComponent.h"
#include "../../DirectXClasses/Buffers/BufferDataTypes.h"


using namespace DirectX::SimpleMath;

class Transform : public AComponent
{
public:
	Transform();
	~Transform();
	
	void Clone(AComponent* copy) override;
	void Perform() override;
	void SetEnabled(bool flag) override;

	TMatrix GetTransformationMatrix();
	void RecalculateChildTransformWithoutParent();
	void RecalculateChildTransformWithParent(const Transform* parent);

	#pragma region Getters Setters
	Vector3 GetPosition();
	void SetPosition(const Vector3& newPos);
	__declspec(property(get = GetPosition, put = SetPosition)) Vector3 Position;

	Vector3 GetLocalPosition();
	void SetLocalPosition(const Vector3& newPos);
	__declspec(property(get = GetLocalPosition, put = SetLocalPosition)) Vector3 LocalPosition;


	Vector3 GetLocalScale();
	void SetLocalScale(const Vector3& newScale);
	__declspec(property(get = GetLocalScale, put = SetLocalScale)) Vector3 LocalScale;


	Vector3 GetEulerAngles();
	Vector3 GetLocalEulerAngles();
	Vector3 GetLocalRight();
	Vector3 GetLocalUp();
	Vector3 GetLocalForward();
	#pragma endregion

	void ScaleUniformly(float factor);

	void Rotate(float xAngleInDeg, float yAngleInDeg, float zAngleInDeg);
	void Rotate(const Vector3& eulerInDeg);
	//void Rotate(const Vector3& axis, float angleInDeg);

private:
	void UpdateTransformationMatrix();
	void UpdateGlobalScaleWithChildren();

	void RotateFromParent(const Vector3& eulerInDeg, const Quaternion& toRotate, const Transform* parent);
	//void RotateFromParent(const Quaternion& toRotate, const Transform* parent);

	void UpdateLocalEulerAnglesWithChildren(const Transform* parent);
	void UpdateLocalVectors();


private:
	Vector3 globalPos;
	Vector3 localPos;

	Vector3 globalScale;
	Vector3 localScale;

	Vector3 eulerAngles;
	Vector3 localEulerAngles;
	Quaternion orientation;

	Vector3 localRight;
	Vector3 localUp;
	Vector3 localForward;

	TMatrix tMatrix;

private:
	friend class InspectorUI;
};