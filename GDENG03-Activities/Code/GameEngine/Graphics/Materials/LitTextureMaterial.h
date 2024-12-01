#pragma once
#include "AMaterial.h"
#include "DirectXClasses/Texture.h"
#include "DirectXClasses/TextureSampler.h"


class LitTextureMaterial : public AMaterial
{
public:
	LitTextureMaterial(std::string textureName);
	~LitTextureMaterial();

	void Render() override;
	void RenderUI() override;

	std::string GetTextureName();


private:
	Texture* texture;
	TextureSampler* sampler;

	std::string textureName;
};