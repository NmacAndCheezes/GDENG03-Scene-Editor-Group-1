#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::Texture(GraphicsEngine* gfx, std::string imagePath) : AD3D11Object(gfx), imagePath(STANDARD_TEXTURE_PATH + imagePath),
    width(0), height(0), colorChannels(0)
{

}

Texture::~Texture()
{

}

bool Texture::Init()
{
    // load up texture from disk
    //stbi_set_flip_vertically_on_load(true);
    unsigned char* tex_bytes = stbi_load(imagePath.c_str(), &width, &height, &colorChannels, 4);

    // create the texture itself with directx
    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = tex_bytes;
    sd.SysMemPitch = width * 4;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
    if (FAILED(gfx->GetDevice()->CreateTexture2D(&textureDesc, &sd, &pTexture))) return false;

    // create resource view on the texture
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    if (FAILED(gfx->GetDevice()->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pTextureView))) return false;
     
    // cleanup
    pTexture.Get()->Release();
    stbi_image_free(tex_bytes);

    return true;
}

void Texture::BindToPipeline()
{
    gfx->GetDeviceContext()->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
}

bool Texture::Release()
{
    if (pTextureView.Get())
    {
        pTextureView.Get()->Release();
    }

    delete this;

    return true;
}

int Texture::GetWidth()
{
    return width;
}

int Texture::GetHeight()
{
    return height;
}

ID3D11ShaderResourceView* Texture::GetTextureView()
{
    return pTextureView.Get();
}
