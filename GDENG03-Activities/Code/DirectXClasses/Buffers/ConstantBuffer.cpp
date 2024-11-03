#pragma once
#include "ConstantBuffer.h"


template<typename T>
ConstantBuffer<T>::ConstantBuffer(GraphicsEngine* gfx, const UINT& registerSlot) : AD3D11Object(gfx), registerSlot(registerSlot)
{

}

template<typename T>
ConstantBuffer<T>::~ConstantBuffer()
{

}

template<typename T>
bool ConstantBuffer<T>::Init()
{
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(T);
	cbd.StructureByteStride = 0u;

	// reserve gpu space only, no values
	/*D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &constants;*/
	
	if (FAILED(gfx->GetDevice()->CreateBuffer(&cbd, nullptr, &pConstantBuffer))) 
	{
		return false;
	}

	return true;
}

template<typename T>
bool ConstantBuffer<T>::Release()
{
	if (pConstantBuffer.Get())
	{
		pConstantBuffer.Get()->Release();
	}

	delete this;
	
	return true;
}

template<typename T>
bool ConstantBuffer<T>::SetConstants(const T& constants)
{
	D3D11_MAPPED_SUBRESOURCE msr;

	if (FAILED(gfx->GetDeviceContext()->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr)))
	{
		return false;
	}

	memcpy(msr.pData, &constants, sizeof(constants));
	gfx->GetDeviceContext()->Unmap(pConstantBuffer.Get(), 0u);

	return true;
}


template<typename T>
void VertexConstantBuffer<T>::BindToPipeline()
{
	gfx->GetDeviceContext()->VSSetConstantBuffers(registerSlot, 1u, pConstantBuffer.GetAddressOf());
}


template<typename T>
void GeometryConstantBuffer<T>::BindToPipeline()
{
	gfx->GetDeviceContext()->GSSetConstantBuffers(registerSlot, 1u, pConstantBuffer.GetAddressOf());
}


template<typename T>
void PixelConstantBuffer<T>::BindToPipeline()
{
	gfx->GetDeviceContext()->PSSetConstantBuffers(registerSlot, 1u, pConstantBuffer.GetAddressOf());
}