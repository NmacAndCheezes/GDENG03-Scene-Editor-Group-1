#include "Shader.h"
#include <d3dcompiler.h>
#include <algorithm>
#include <string>


#pragma region Shader methods

LPCWSTR Shader::shaderFolderDir = L"";

Shader::Shader(GraphicsEngine* gfx, LPCWSTR shaderFileName) : AD3D11Object(gfx), shaderFileName(shaderFileName)
{

}

Shader::~Shader()
{

}

bool Shader::Release()
{
	delete this;
	return true;
}

LPCWSTR Shader::GetShaderFileName()
{
	/*std::wstring wStr(shaderFile);
	wStr.erase(wStr.find(L".cso"), 4);
	LPCWSTR shaderName = wStr.c_str();
	return shaderName;*/

	return shaderFileName;
}

void Shader::SetShaderFolderDirectory(LPCWSTR dir)
{
	shaderFolderDir = dir;
}
#pragma endregion


#pragma region VertexShader methods
VertexShader::VertexShader(GraphicsEngine* gfx, LPCWSTR shaderFileName, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements) : Shader(gfx, shaderFileName), inputElements(inputElements)
{

}

VertexShader::~VertexShader()
{

}

bool VertexShader::Init()
{
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob; 

	std::wstring shaderPath = std::wstring(shaderFolderDir) + shaderFileName;
	 
	if (FAILED(D3DReadFileToBlob(shaderPath.c_str(), &shaderBlob)))
	{
		return false;
	}
	 
	if (FAILED(gfx->GetDevice()->CreateVertexShader(shaderBlob.Get()->GetBufferPointer(), shaderBlob.Get()->GetBufferSize(), nullptr, &vShader)))
	{
		return false;
	}

	if (FAILED(gfx->GetDevice()->CreateInputLayout(inputElements.data(), (UINT)inputElements.size(), 
		shaderBlob.Get()->GetBufferPointer(), shaderBlob.Get()->GetBufferSize(), &inputLayout)))
	{
		return false;
	}

	shaderBlob.Get()->Release(); 

	return true;
}

void VertexShader::BindToPipeline()
{
	gfx->GetDeviceContext()->IASetInputLayout(inputLayout.Get());
	gfx->GetDeviceContext()->VSSetShader(vShader.Get(), nullptr, 0u);
}

bool VertexShader::Release()
{
	if (vShader.Get())
	{
		vShader.Get()->Release();
	}

	if (inputLayout.Get())
	{
		inputLayout.Get()->Release();
	}

	inputElements.clear(); 
	inputElements.shrink_to_fit();

	Shader::Release();

	return true;
}
#pragma endregion


#pragma region PixelShader methods
PixelShader::PixelShader(GraphicsEngine* gfx, LPCWSTR shaderFileName) : Shader(gfx, shaderFileName)
{

}

PixelShader::~PixelShader()
{

}

bool PixelShader::Init()
{
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;

	std::wstring shaderPath = std::wstring(shaderFolderDir) + shaderFileName;
	 
	if (FAILED(D3DReadFileToBlob(shaderPath.c_str(), &shaderBlob)))
	{
		return false;
	}

	if (FAILED(gfx->GetDevice()->CreatePixelShader(shaderBlob.Get()->GetBufferPointer(), shaderBlob.Get()->GetBufferSize(), nullptr, &pShader)))
	{
		return false;
	}
	 
	shaderBlob.Get()->Release(); 

	return true;
}

void PixelShader::BindToPipeline()
{
	gfx->GetDeviceContext()->PSSetShader(pShader.Get(), nullptr, 0u); 
}

bool PixelShader::Release()
{
	pShader.Get()->Release();
	Shader::Release();

	return true;
}
#pragma endregion