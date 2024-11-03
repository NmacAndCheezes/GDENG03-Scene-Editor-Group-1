#pragma once
#include <wrl.h>
#include <vector>
#include "../AD3D11Object.h"


template<typename T>
class VertexBuffer : public AD3D11Object
{
public:
	VertexBuffer(GraphicsEngine* gfx, const std::vector<T>& vertices);
	~VertexBuffer();

	bool Init() override;
	void BindToPipeline() override;
	bool Release() override;


private:
	UINT stride; 
	UINT listSize;
	std::vector<T> vertices;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};