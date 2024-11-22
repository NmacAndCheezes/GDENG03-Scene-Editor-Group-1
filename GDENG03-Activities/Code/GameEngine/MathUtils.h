#pragma once
#include <SimpleMath.h>
#include <cmath>
#include <reactphysics3d/reactphysics3d.h>

using namespace DirectX::SimpleMath;

class MathUtils
{
public:
	static bool ApproximatelyEqualTo(const float toBeCompared, const float toCompareWith, const float tolerance = DEFAULT_EQUALITY_TOLERANCE); 

	static bool IsWithinRange(const float value, const float min, const float max);
	static float Clamp(float value, const float min, const float max);
	static int Clamp(int value, const int min, const int max);
	static float Clamp01(float value);
	static int Clamp01(int value);

	static float Lerp(const float a, const float b, const float t); 
	static float Map(const float valueFrom1, const float range1Min, const float range1Max, const float range2Min, const float range2Max);

	static float RandFloatWithRange(const float min = 0.f, const float max = 1.f);
	static int RandIntWithRange(const int min = 0, const int max = 1);

	static Vector3 CheckDivisionByZero(const Vector3& dividend, const Vector3& divisor);

	static float GetNearestReferenceAngle(const float angle);
	static Vector3 GetNearestReferenceAngles(const Vector3& eulerAngle);

	static rp3d::Vector3 ConvertVector(const Vector3& vec);
	static Vector3 ConvertVector(const rp3d::Vector3& vec);
	static rp3d::Quaternion ConvertQuaternion(const Quaternion& quat);
	static Quaternion ConvertQuaternion(const rp3d::Quaternion& quat);

public:
	static const float Deg2Rad;
	static const float Rad2Deg;

private:
	static const float DEFAULT_EQUALITY_TOLERANCE;
};