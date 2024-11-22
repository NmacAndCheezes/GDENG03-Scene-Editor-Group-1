#pragma once
#include "AMesh.h"
#include "../MathUtils.h"


class CubeMesh : public AMesh
{
public:
	std::vector<Vector3> GetVertexPositions() override
	{
		std::vector<Vector3> vertices(8);
		vertices[0] = { -0.5f,  0.5f, -0.5f };
		vertices[1] = { 0.5f,  0.5f, -0.5f };
		vertices[2] = { -0.5f, -0.5f, -0.5f };
		vertices[3] = { 0.5f, -0.5f, -0.5f };
		vertices[4] = { 0.5f,  0.5f,  0.5f };
		vertices[5] = { -0.5f,  0.5f,  0.5f };
		vertices[6] = { 0.5f, -0.5f,  0.5f };
		vertices[7] = { -0.5f, -0.5f,  0.5f };

		return vertices;
	}

	std::vector<unsigned short> GetIndices() override
	{
		std::vector<unsigned short> indices = {
			0, 1, 2, // front
			1, 3, 2,
			4, 5, 6, // back
			5, 7, 6,
			5, 4, 0, // top
			4, 1, 0,
			2, 3, 7, // bottom
			3, 6, 7,
			1, 4, 3, // right
			4, 6, 3,
			5, 0, 7, // left
			0, 2, 7
		};

		return indices;
	}
};