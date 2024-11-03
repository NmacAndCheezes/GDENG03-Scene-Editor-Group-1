#pragma once
#include <SimpleMath.h>
#include <cmath>

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

	static float RandFloatWithRange(const float min, const float max);
	static int RandIntWithRange(const int min, const int max);

	static Vector3 CheckDivisionByZero(const Vector3& dividend, const Vector3& divisor);


public:
	static const float Deg2Rad;
	static const float Rad2Deg;

private:
	static const float DEFAULT_EQUALITY_TOLERANCE;
};