#pragma once
#include "AMesh.h"


template <typename T>
class CubeMesh : public AMesh<T>
{
public:
	VertexBuffer<T>* CreateVertexBuffer() override
	{
		std::vector<T> vertices(8);
		vertices[0].pos = { -0.5f,  0.5f, -0.5f };
		vertices[1].pos = {  0.5f,  0.5f, -0.5f };
		vertices[2].pos = { -0.5f, -0.5f, -0.5f };
		vertices[3].pos = {  0.5f, -0.5f, -0.5f };
		vertices[4].pos = {  0.5f,  0.5f,  0.5f };
		vertices[5].pos = { -0.5f,  0.5f,  0.5f };
		vertices[6].pos = { 0.5f, -0.5f,  0.5f };
		vertices[7].pos = { -0.5f, -0.5f,  0.5f };

		vertices[0].vColor = { 1.0f, 1.0f, 1.0f };
		vertices[1].vColor = { 1.0f, 1.0f, 1.0f };
		vertices[2].vColor = { 1.0f, 1.0f, 1.0f };
		vertices[3].vColor = { 1.0f, 1.0f, 1.0f };
		vertices[4].vColor = { 1.0f, 1.0f, 1.0f };
		vertices[5].vColor = { 1.0f, 1.0f, 1.0f };
		vertices[6].vColor = { 1.0f, 1.0f, 1.0f }; 
		vertices[7].vColor = { 1.0f, 1.0f, 1.0f };

		return new VertexBuffer<T>(GraphicsEngine::GetInstance(), vertices);
	}

	IndexBuffer* CreateIndexBuffer() override 
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
		return new IndexBuffer(GraphicsEngine::GetInstance(), indices); 
	}

	/*VertexBuffer<T>* WithColors()
	{
		std::vector<T> data(8);
		data[0].pos = { -0.5f,  0.5f, -0.5f };
		data[1].pos = { 0.5f,  0.5f, -0.5f };
		data[2].pos = { -0.5f, -0.5f, -0.5f };
		data[3].pos = { 0.5f, -0.5f, -0.5f };
		data[4].pos = { 0.5f,  0.5f,  0.5f };
		data[5].pos = { -0.5f,  0.5f,  0.5f };
		data[6].pos = { 0.5f, -0.5f,  0.5f };
		data[7].pos = { -0.5f, -0.5f,  0.5f };

		data[0].vColor = { 1.0f, 0.0f, 0.0f };
		data[1].vColor = { 0.0f, 1.0f, 0.0f };
		data[2].vColor = { 0.0f, 0.0f, 1.0f };
		data[3].vColor = { 1.0f, 1.0f, 0.0f };
		data[4].vColor = { 0.0f, 1.0f, 0.0f };
		data[5].vColor = { 1.0f, 0.0f, 0.0f };
		data[6].vColor = { 1.0f, 1.0f, 0.0f };
		data[7].vColor = { 0.0f, 0.0f, 1.0f };

		return new VertexBuffer<T>(GraphicsEngine::GetInstance(), data);
	}*/
};