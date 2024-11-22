#pragma once
#include "AMesh.h"
#include "../MathUtils.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

class CylinderMesh : public AMesh
{
public:
	float numPointsOnCircle = 20;

	std::vector<Vector3> GetVertexPositions() override
	{
		int total = numPointsOnCircle * 2 + 2;
		std::vector<Vector3> vertices(total); 

		vertices[0] = { 0.0f, -1.0f, 0.0f };
		vertices[total / 2] = { 0.0f, 1.0f, 0.0f };

		float currAngle = 0;
		float angleIncrements = 360.f / (numPointsOnCircle);

		for (int i = 1; i <= numPointsOnCircle; i++)
		{
			float x = sin(currAngle * MathUtils::Deg2Rad) / 2.f;
			float z = cos(currAngle * MathUtils::Deg2Rad) / 2.f;

			vertices[i] = { x, -1.0f, z };
			vertices[i + total / 2] = { x, 1.0f, z };

			currAngle += angleIncrements;
		}

		return vertices;
	}

	std::vector<unsigned short> GetIndices() override
	{
		std::vector<unsigned short> indices; 

		int halfSize = numPointsOnCircle + 1; 

		for (int i = 0; i < numPointsOnCircle; i++) 
		{
			int bottomA = i + 1; 
			int bottomB = (i == numPointsOnCircle - 1) ? 1 : i + 2; 
			int topA = halfSize + i + 1; 
			int topB = (i == numPointsOnCircle - 1) ? halfSize + 1 : halfSize + i + 2; 

			// insert bottom circle triangle
			indices.push_back(bottomB); 
			indices.push_back(bottomA); 
			indices.push_back(0); 
			// insert top circle triangle
			indices.push_back(halfSize); 
			indices.push_back(topA); 
			indices.push_back(topB); 
			// insert side triangle A
			indices.push_back(bottomB); 
			indices.push_back(topA); 
			indices.push_back(bottomA); 
			// insert side triangle B
			indices.push_back(topA); 
			indices.push_back(bottomB); 
			indices.push_back(topB); 
		}

		return indices;
	}
};