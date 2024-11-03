#pragma once
#include "AMesh.h"
#include "../MathUtils.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

template <typename T>
class CylinderMesh : public AMesh<T>
{
public:
	float numPointsOnCircle = 20;

	VertexBuffer<T>* CreateVertexBuffer() override
	{
		int total = numPointsOnCircle * 2 + 2; 
		std::vector<T> vertices(total); 

		vertices[0].pos = { 0.0f, -1.0f, 0.0f }; 
		vertices[0].vColor = { 1.0f, 1.0f, 1.0f };
		vertices[total / 2].pos = { 0.0f, 1.0f, 0.0f };
		vertices[total / 2].vColor = { 1.0f, 1.0f, 1.0f };

		float currAngle = 0;
		float angleIncrements = 360.f / (numPointsOnCircle);

		for (int i = 1; i <= numPointsOnCircle; i++)
		{
			float x = sin(currAngle * MathUtils::Deg2Rad) / 2.f;
			float z = cos(currAngle * MathUtils::Deg2Rad) / 2.f;  

			vertices[i].pos = { x, -1.0f, z }; 
			vertices[i].vColor = { 1.0f, 1.0f, 1.0f };   
			vertices[i + total / 2].pos = { x, 1.0f, z }; 
			vertices[i + total / 2].vColor = { 1.0f, 1.0f, 1.0f };  

			currAngle += angleIncrements;
		}

		return new VertexBuffer<T>(GraphicsEngine::GetInstance(), vertices);
	}

	IndexBuffer* CreateIndexBuffer() override
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

		return new IndexBuffer(GraphicsEngine::GetInstance(), indices);
	}
};
