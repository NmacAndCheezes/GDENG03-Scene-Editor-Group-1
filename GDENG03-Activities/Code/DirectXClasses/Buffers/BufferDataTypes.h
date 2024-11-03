#pragma once
#include <SimpleMath.h>


using namespace DirectX::SimpleMath;

struct VertexData
{
	Vector3 pos;
	Vector3 vColor;

	VertexData() : pos(0.0f, 0.0f, 0.0f), vColor(0.0f, 0.0f, 0.0f)
	{
	}

	VertexData(const Vector3& pos, const Vector3& vColor) : pos(pos), vColor(vColor)
	{
	}

	VertexData(const VertexData& copy) : pos(copy.pos), vColor(copy.vColor)
	{
	}
};

__declspec(align(16))
struct TMatrix
{
	Matrix transform;
};

__declspec(align(16))
struct VPMatrix
{
	Matrix view;
	Matrix projection;
};