#pragma once
#include "AMesh.h"
#include "../MathUtils.h"


class QuadMesh : public AMesh
{
public:
	std::vector<Vector3> GetVertexPositions() override
	{
		std::vector<Vector3> vertices(4);
		vertices[0] = { -0.5f,  0.5f, 0.0f };
		vertices[1] = { 0.5f,  0.5f, 0.0f };
		vertices[2] = { -0.5f, -0.5f, 0.0f };
		vertices[3] = { 0.5f, -0.5f, 0.0f };

		return vertices;
	}

	std::vector<unsigned short> GetIndices() override
	{
		std::vector<unsigned short> indices = { 
			0, 1, 2,
			1, 3, 2
		};

		return indices;
	}
};