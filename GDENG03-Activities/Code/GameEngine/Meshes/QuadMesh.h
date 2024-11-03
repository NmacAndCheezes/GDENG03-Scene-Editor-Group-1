#pragma once
#include "AMesh.h"


template <typename T>
class QuadMesh : public AMesh<T>
{
public:
	VertexBuffer<T>* CreateVertexBuffer() override
	{
		std::vector<T> vertices(4); 
		vertices[0].pos = { -0.5f,  0.5f, 0.0f };
		vertices[1].pos = { 0.5f,  0.5f, 0.0f };
		vertices[2].pos = { -0.5f, -0.5f, 0.0f };
		vertices[3].pos = { 0.5f, -0.5f, 0.0f };
		vertices[0].vColor = { 1.0f, 1.0f, 1.0f };
		vertices[1].vColor = { 1.0f, 1.0f, 1.0f };
		vertices[2].vColor = { 1.0f, 1.0f, 1.0f };
		vertices[3].vColor = { 1.0f, 1.0f, 1.0f };

		/*std::vector<T> data = {
			{{-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
			{{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
			{{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
			{{ 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
		};*/

		return new VertexBuffer<T>(GraphicsEngine::GetInstance(), vertices);
	}

	IndexBuffer* CreateIndexBuffer() override
	{
		std::vector<unsigned short> indices = {
			0, 1, 2,
			1, 3, 2
		};
		return new IndexBuffer(GraphicsEngine::GetInstance(), indices);
	}
};