#include "LitTextureMaterial.h"
#include "../GraphicsEngine.h"


LitTextureMaterial::LitTextureMaterial(std::string textureName) : AMaterial(L"LitTextureShader")
{
    texture = new Texture(GraphicsEngine::GetInstance(), textureName);
    texture->Init();

    sampler = new TextureSampler(GraphicsEngine::GetInstance());
    sampler->Init();
}

LitTextureMaterial::~LitTextureMaterial()
{
    texture->Release();
    sampler->Release();
}

void LitTextureMaterial::Render()
{
	texture->BindToPipeline();
	sampler->BindToPipeline();
}