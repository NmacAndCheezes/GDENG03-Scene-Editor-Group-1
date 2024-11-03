#pragma once
#include <wrl.h>
#include <vector>
#include "AD3D11Object.h"


class Shader : public AD3D11Object
{
public:
	Shader(GraphicsEngine* gfx, LPCWSTR shaderFileName);
	~Shader();

	virtual bool Release() override;
	LPCWSTR GetShaderFileName();
	static void SetShaderFolderDirectory(LPCWSTR dir);


protected:
	static LPCWSTR shaderFolderDir;
	LPCWSTR shaderFileName;
};


class VertexShader : public Shader
{
public:
	VertexShader(GraphicsEngine* gfx, LPCWSTR shaderFileName, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements);
	~VertexShader();

	bool Init() override;
	void BindToPipeline() override;
	bool Release() override;


private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vShader;
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};


class PixelShader : public Shader
{
public:
	PixelShader(GraphicsEngine* gfx, LPCWSTR shaderFileName);
	~PixelShader();

	bool Init() override;
	void BindToPipeline() override;
	bool Release() override;


private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pShader;
};