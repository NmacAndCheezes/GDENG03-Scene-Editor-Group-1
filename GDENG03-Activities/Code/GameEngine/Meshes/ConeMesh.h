#pragma once
#include "AMesh.h"
#include "../MathUtils.h"

class ConeMesh : public AMesh
{
public:
	int numPointsOnCircle = 20;

	std::vector<Vector3> GetVertexPositions() override
	{
		std::vector<Vector3> vertices(numPointsOnCircle + 2); 
		vertices[0] = Vector3(0.0f, -0.5f, 0.0f);

		float currAngle = 0; 
		float angleIncrements = 360.f / (float)(numPointsOnCircle); 

		for (int i = 1; i <= numPointsOnCircle; i++) 
		{
			float x = sin(currAngle * M_PI / 180.f) / 2.f; 
			float z = cos(currAngle * M_PI / 180.f) / 2.f; 
			vertices[i] = Vector3(x, -0.5f, z);

			currAngle += angleIncrements;
		}

		vertices[numPointsOnCircle + 1] = Vector3(0.0f, 0.5f, 0.0f);

		return vertices;
	}

	std::vector<unsigned short> GetIndices() override
	{
		std::vector<unsigned short> indices; 

		for (int i = 0; i < numPointsOnCircle; i++) 
		{
			// insert circle triangle
			indices.push_back((i == numPointsOnCircle - 1) ? 1 : i + 2); 
			indices.push_back(i + 1); 
			indices.push_back(0); 
			// insert side triangle
			indices.push_back(numPointsOnCircle + 1); 
			indices.push_back(i + 1); 
			indices.push_back((i == numPointsOnCircle - 1) ? 1 : i + 2); 
		}
		return indices;
	}
};