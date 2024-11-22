#pragma once
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;


class AMesh
{
public:
	virtual std::vector<Vector3> GetVertexPositions() = 0;
	virtual std::vector<unsigned short> GetIndices() = 0;
};