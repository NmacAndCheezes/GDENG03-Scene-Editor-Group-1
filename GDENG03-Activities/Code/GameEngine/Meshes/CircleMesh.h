#pragma once
#include "AMesh.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

template <typename T>
class CircleMesh : public AMesh<T>
{
public:
	int size = 21;

	VertexBuffer<T>* CreateVertexBuffer() override
	{
		std::vector<T> vertices(size);
		vertices[0].pos = Vector3::Zero;
		vertices[0].vColor = { 1.0f, 1.0f, 1.0f };

		float currAngle = 0;
		float angleIncrements = 360.f / (size - 1.f);

		for (int i = 1; i < size; i++)
		{
			float x = sin(currAngle * M_PI / 180.f) / 2.f;
			float y = cos(currAngle * M_PI / 180.f) / 2.f;
			Vector3 newVert = Vector3(x, y, 0.0f);

			vertices[i].pos = newVert;
			vertices[i].vColor = { 1.0f, 1.0f, 1.0f };

			currAngle += angleIncrements;
		}

		return new VertexBuffer<T>(GraphicsEngine::GetInstance(), vertices);
	}

	IndexBuffer* CreateIndexBuffer() override
	{
		std::vector<unsigned short> indices;

		for (int i = 0; i < size - 1; i++)
		{
			indices.push_back(0); 
			indices.push_back(i + 1); 
			indices.push_back((i == size - 2) ? 1 : i + 2);
		}

		return new IndexBuffer(GraphicsEngine::GetInstance(), indices);
	}
};