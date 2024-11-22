#pragma once
#include "AMesh.h"
#include "../MathUtils.h"

class CircleMesh : public AMesh
{
public:
	int size = 21;

	std::vector<Vector3> GetVertexPositions() override
	{
		std::vector<Vector3> vertices(size);
		vertices[0] = Vector3::Zero;

		float currAngle = 0;
		float angleIncrements = 360.f / (size - 1.f);

		for (int i = 1; i < size; i++)
		{
			float x = sin(currAngle * M_PI / 180.f) / 2.f;
			float y = cos(currAngle * M_PI / 180.f) / 2.f;
			vertices[i] = Vector3(x, y, 0.0f);

			currAngle += angleIncrements;
		}

		return vertices;
	}

	std::vector<unsigned short> GetIndices() override
	{
		std::vector<unsigned short> indices; 

		for (int i = 0; i < size - 1; i++) 
		{
			indices.push_back(0); 
			indices.push_back(i + 1); 
			indices.push_back((i == size - 2) ? 1 : i + 2); 
		}

		return indices;
	}
};