#pragma once
#include <unordered_map>
#include "../../DirectXClasses/Shader.h"


class ShaderManager
{
public:
	struct ShaderProgram
	{
		LPCWSTR shaderType;
		VertexShader* vShader;
		PixelShader* pShader;
	};


public:
	static ShaderManager* GetInstance();
	ShaderManager(const ShaderManager&) = delete; 
	ShaderManager& operator=(const ShaderManager&) = delete; 

	bool CreateVertexShader(LPCWSTR shaderFile, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements);
	bool CreatePixelShader(LPCWSTR shaderFile);
	bool CreateShaderProgram(LPCWSTR shaderType, LPCWSTR vShaderName, LPCWSTR pShaderName);
	std::vector<ShaderProgram> GetShaderProgramsList();

private:
	ShaderManager() {};
	~ShaderManager();


private:
	static ShaderManager* sharedInstance;

	std::vector<ShaderProgram> shaderProgramsList;
	std::unordered_map<LPCWSTR, VertexShader*> vShadersList;
	std::unordered_map<LPCWSTR, PixelShader*> pShadersList;
};