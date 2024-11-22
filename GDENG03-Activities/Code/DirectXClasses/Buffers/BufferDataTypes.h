#pragma once
#include <SimpleMath.h>


using namespace DirectX::SimpleMath;

struct VUnlitColorData 
{
	Vector3 pos;
	Vector3 vColor;

	VUnlitColorData() : pos(0.0f, 0.0f, 0.0f), vColor(0.0f, 0.0f, 0.0f)
	{
	}

	VUnlitColorData(const Vector3& pos, const Vector3& vColor) : pos(pos), vColor(vColor)
	{
	}

	VUnlitColorData(const VUnlitColorData& copy) : pos(copy.pos), vColor(copy.vColor)
	{
	}
};

struct VLitTextureData 
{
	Vector3 pos;
	Vector3 normals;
	Vector2 uv;

	VLitTextureData() : pos(0.0f, 0.0f, 0.0f), normals(0.0f, 0.0f, 0.0f), uv(0.0f, 0.0f)
	{
	}

	VLitTextureData(const Vector3& pos, const Vector3& normals, const Vector2& uv) : pos(pos), normals(normals), uv(uv) 
	{
	}

	VLitTextureData(const VLitTextureData& copy) : pos(copy.pos), normals(copy.normals), uv(copy.uv) 
	{
	}
};

struct GenericVertexData
{
	Vector3 pos;
	Vector3 normals;
	Vector2 uv;
	Vector3 vColor;

	GenericVertexData() : pos(0.0f, 0.0f, 0.0f), normals(0.0f, 0.0f, 0.0f), uv(0.0f, 0.0f), vColor(1.0f, 1.0f, 1.0f)
	{
	}

	GenericVertexData(const Vector3& pos, const Vector3& normals, const Vector2& uv) : pos(pos), normals(normals), uv(uv), vColor(vColor)
	{
	}

	GenericVertexData(const GenericVertexData& copy) : pos(copy.pos), normals(copy.normals), uv(copy.uv), vColor(copy.vColor)
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