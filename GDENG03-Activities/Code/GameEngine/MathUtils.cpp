#include "MathUtils.h"


const float MathUtils::Deg2Rad = (float)M_PI / 180.f;
const float MathUtils::Rad2Deg = 180.f / (float)M_PI;
const float MathUtils::DEFAULT_EQUALITY_TOLERANCE = 1e-5f;


bool MathUtils::ApproximatelyEqualTo(const float toBeCompared, const float toCompareWith, const float tolerance)
{
    return (toBeCompared <= toCompareWith + tolerance) && (toBeCompared >= toCompareWith - tolerance);
}

bool MathUtils::IsWithinRange(const float value, const float min, const float max)
{
	if (min == max || min > max) return false;
	return (value > min && value < max);
}

float MathUtils::Clamp(float value, const float min, const float max)
{
	if (min == max) return min;
	else if (max < min) return value;

	if (value < min) value = min;
	if (value > max) value = max;
	return value;
}

int MathUtils::Clamp(int value, const int min, const int max)
{
	if (min == max) return min;
	else if (max < min) return value;

	if (value < min) value = min; 
	if (value > max) value = max; 
	return value; 
}

float MathUtils::Clamp01(float value)
{
	if (value < 0.0f) value = 0.0f;
	else if (value > 1.0f) value = 1.0f;
	return value; 
}

int MathUtils::Clamp01(int value)
{
	if (value < 0) value = 0;
	else if (value > 1) value = 1;
	return value;
}

float MathUtils::Lerp(const float a, const float b, const float t)
{
	return (b + t * (a - b));
}

float MathUtils::Map(const float valueFrom1, const float range1Min, const float range1Max, const float range2Min, const float range2Max)
{
	return ((valueFrom1 - range1Min) / (range1Max - range1Min)) * (range2Max - range2Min) + range2Min;
}

float MathUtils::RandFloatWithRange(const float min, const float max)
{
	return (float)rand() / (float)RAND_MAX * (max - min) + min;
}

int MathUtils::RandIntWithRange(const int min, const int max)
{
	return rand() % (max - min + 1) + min;
}

Vector3 MathUtils::CheckDivisionByZero(const Vector3& dividend, const Vector3& divisor)
{
	// Vector3 division from SimpleMath defaults to 'inf' value if there's division by 0
	Vector3 result;
	result.x = (divisor.x == 0) ? 0 : (dividend.x / divisor.x);
	result.y = (divisor.y == 0) ? 0 : (dividend.y / divisor.y);
	result.z = (divisor.z == 0) ? 0 : (dividend.z / divisor.z);
	return result;
}