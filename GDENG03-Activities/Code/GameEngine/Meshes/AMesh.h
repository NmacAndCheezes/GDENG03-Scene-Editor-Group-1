#pragma once
#include "../../DirectXClasses/Buffers/VertexBuffer.cpp"
#include "../../DirectXClasses/Buffers/IndexBuffer.h"


template <typename T>
class AMesh
{
public:
	virtual VertexBuffer<T>* CreateVertexBuffer() = 0;
	virtual IndexBuffer* CreateIndexBuffer() = 0;
};