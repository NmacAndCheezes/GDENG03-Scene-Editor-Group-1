#pragma once
#include "VertexBuffer.h"


template<typename T>
VertexBuffer<T>::VertexBuffer(GraphicsEngine* gfx, const std::vector<T>& vertices) : AD3D11Object(gfx), vertices(vertices)
{
	stride = sizeof(T);
	listSize = vertices.size();
}

template<typename T>
VertexBuffer<T>::~VertexBuffer()
{
	
}

template<typename T>
bool VertexBuffer<T>::Init()
{
	D3D11_BUFFER_DESC bufferDesc = {}; 
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	bufferDesc.Usage = D3D11_USAGE_DEFAULT; 
	bufferDesc.CPUAccessFlags = 0u; 
	bufferDesc.MiscFlags = 0u; 
	bufferDesc.ByteWidth = UINT(stride * listSize); 
	bufferDesc.StructureByteStride = stride; 

	D3D11_SUBRESOURCE_DATA srData = {}; 
	srData.pSysMem = vertices.data(); 

	if (FAILED(gfx->GetDevice()->CreateBuffer(&bufferDesc, &srData, &pVertexBuffer)))
	{
		return false;
	}

	//vertices.clear();
	//vertices.shrink_to_fit();

	return true;
}

template<typename T>
void VertexBuffer<T>::BindToPipeline()
{
	UINT offset = 0u;
    gfx->GetDeviceContext()->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}

template<typename T>
bool VertexBuffer<T>::Release()
{
	if (pVertexBuffer.Get())
	{
		pVertexBuffer.Get()->Release(); 
	}

	delete this;

    return true;
}