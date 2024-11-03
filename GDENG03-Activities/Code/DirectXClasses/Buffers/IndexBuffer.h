#pragma once
#include <wrl.h>
#include <vector>
#include "../AD3D11Object.h"


class IndexBuffer : public AD3D11Object
{
public:
	IndexBuffer(GraphicsEngine* gfx, const std::vector<unsigned short>& indices);
	~IndexBuffer();

	bool Init() override;
	void BindToPipeline() override;
	bool Release() override;
	UINT GetIndexCount();


private:
	UINT stride;
	UINT listSize;
	std::vector<unsigned short> indices;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};