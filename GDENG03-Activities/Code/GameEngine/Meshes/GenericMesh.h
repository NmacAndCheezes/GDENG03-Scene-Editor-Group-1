#pragma once
#include <SimpleMath.h>
#include "AMesh.h"
#include <DirectXClasses/Buffers/BufferDataTypes.h>
using namespace DirectX::SimpleMath;


class GenericMesh : public AMesh
{
public:
	GenericMesh(std::vector<Vector3> vertices, std::vector<unsigned short> indices)
	{
		this->vertices = vertices;
		this->indices = indices;
	}

	GenericMesh(std::vector<GenericVertexData> vertices, std::vector<unsigned short> indices)
	{
		std::vector<Vector3> verticesPos;
		for (auto& i : vertices)
		{
			verticesPos.push_back(i.pos);
		}
		this->vertices = verticesPos;
		this->indices = indices;
	}

	virtual std::vector<Vector3> GetVertexPositions() { return vertices; }
	virtual std::vector<unsigned short> GetIndices() { return indices; }

private:
	std::vector<Vector3> vertices;
	std::vector<unsigned short> indices;
};
