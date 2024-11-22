#pragma once
#include <string>
#include "AD3D11Object.h"


class Texture : public AD3D11Object
{
public:
	Texture(GraphicsEngine* gfx, std::string imagePath);
	~Texture();

	bool Init() override;
	void BindToPipeline() override;
	bool Release() override;

	int GetWidth();
	int GetHeight();
	ID3D11ShaderResourceView* GetTextureView();


private:
	const std::string STANDARD_TEXTURE_PATH = "Assets/Textures/";

	std::string imagePath;
	int width;
	int height;
	int colorChannels;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};