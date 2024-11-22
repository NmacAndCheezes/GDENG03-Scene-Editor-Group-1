#pragma once
#include "AD3D11Object.h"


class TextureSampler : public AD3D11Object
{
public:
	TextureSampler(GraphicsEngine* gfx);
	~TextureSampler();

	bool Init() override;
	void BindToPipeline() override;
	bool Release() override;


private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
};