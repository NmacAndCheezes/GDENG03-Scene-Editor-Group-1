#include "IndexBuffer.h"


IndexBuffer::IndexBuffer(GraphicsEngine* gfx, const std::vector<unsigned short>& indices) : AD3D11Object(gfx), indices(indices)
{
	stride = sizeof(unsigned short);
	listSize = (UINT)indices.size();
}

IndexBuffer::~IndexBuffer()
{

}

bool IndexBuffer::Init()
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = UINT(stride * listSize);
	bufferDesc.StructureByteStride = stride;

	D3D11_SUBRESOURCE_DATA srData = {};
	srData.pSysMem = indices.data();

	if (FAILED(gfx->GetDevice()->CreateBuffer(&bufferDesc, &srData, &pIndexBuffer)))
	{
		return false;
	}

	//indices.clear();
	//indices.shrink_to_fit();

	return true;
}

void IndexBuffer::BindToPipeline()
{
	gfx->GetDeviceContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

bool IndexBuffer::Release()
{
	if (pIndexBuffer.Get())
	{
		pIndexBuffer.Get()->Release();
	}

	delete this;

	return true;
}

UINT IndexBuffer::GetIndexCount()
{
	return listSize;
}
