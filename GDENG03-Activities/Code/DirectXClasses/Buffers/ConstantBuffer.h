#pragma once
#include <wrl.h>
#include "../AD3D11Object.h"


template<typename T>
class ConstantBuffer : public AD3D11Object
{
public:
	ConstantBuffer(GraphicsEngine* gfx, const UINT& registerSlot);
	~ConstantBuffer();

	bool Init() override;
	bool Release() override;
	bool SetConstants(const T& constants);


protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	UINT registerSlot;
};


template<typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
private:
	using AD3D11Object::gfx;
	using ConstantBuffer<T>::pConstantBuffer;
	using ConstantBuffer<T>::registerSlot;

public:
	VertexConstantBuffer(GraphicsEngine* gfx, const UINT& registerSlot) : ConstantBuffer<T>(gfx, registerSlot) {}
	~VertexConstantBuffer() {}
	void BindToPipeline() override;
};


template<typename T>
class GeometryConstantBuffer : public ConstantBuffer<T>
{
private:
	using AD3D11Object::gfx;
	using ConstantBuffer<T>::pConstantBuffer;
	using ConstantBuffer<T>::registerSlot;

public:
	GeometryConstantBuffer(GraphicsEngine* gfx, const UINT& registerSlot) : ConstantBuffer<T>(gfx, registerSlot) {}
	~GeometryConstantBuffer() {}
	void BindToPipeline() override;
};


template<typename T>
class PixelConstantBuffer : public ConstantBuffer<T>
{
private:
	using AD3D11Object::gfx;
	using ConstantBuffer<T>::pConstantBuffer;
	using ConstantBuffer<T>::registerSlot;

public:
	PixelConstantBuffer(GraphicsEngine* gfx, const UINT& registerSlot) : ConstantBuffer<T>(gfx, registerSlot) {}
	~PixelConstantBuffer() {}
	void BindToPipeline() override;
};