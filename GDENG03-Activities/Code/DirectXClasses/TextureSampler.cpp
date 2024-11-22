#include "TextureSampler.h"


TextureSampler::TextureSampler(GraphicsEngine* gfx) : AD3D11Object(gfx)
{

}

TextureSampler::~TextureSampler()
{

}

bool TextureSampler::Init()
{
    D3D11_SAMPLER_DESC desc = {};
    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    if (FAILED(gfx->GetDevice()->CreateSamplerState(&desc, &pSampler))) return false;

    return true;
}

void TextureSampler::BindToPipeline()
{
    gfx->GetDeviceContext()->PSSetSamplers(0, 1, pSampler.GetAddressOf());
}

bool TextureSampler::Release()
{
    if (pSampler.Get())
    {
        pSampler.Get()->Release();
    }

    delete this;

    return true;
}
