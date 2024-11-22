#pragma once
#include "AMesh.h"
#include "../MathUtils.h"


class PlaneMesh : public AMesh
{
public:
	std::vector<Vector3> GetVertexPositions() override
	{
		std::vector<Vector3> vertices(4); 
		vertices[0] = { -2.5f, 0.0f,  2.5f }; 
		vertices[1] = { 2.5f, 0.0f,  2.5f }; 
		vertices[2] = { -2.5f, 0.0f, -2.5f }; 
		vertices[3] = { 2.5f, 0.0f, -2.5f }; 

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