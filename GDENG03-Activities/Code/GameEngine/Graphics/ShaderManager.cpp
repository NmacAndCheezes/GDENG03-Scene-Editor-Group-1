#include "ShaderManager.h"


ShaderManager* ShaderManager::sharedInstance = nullptr;

ShaderManager* ShaderManager::GetInstance()
{
    if (sharedInstance == nullptr)
    {
        sharedInstance = new ShaderManager();
    }

    return sharedInstance;
}

ShaderManager::~ShaderManager()
{
    shaderProgramsList.clear();
    shaderProgramsList.shrink_to_fit();

    for (auto& itr : vShadersList)
    {
        itr.second->Release();
    }
    vShadersList.clear();

    for (auto& itr : pShadersList)
    {
        itr.second->Release();
    }
    pShadersList.clear();
}

bool ShaderManager::CreateVertexShader(LPCWSTR shaderFile, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements)
{
    VertexShader* vShader = new VertexShader(GraphicsEngine::GetInstance(), shaderFile, inputElements);
    
    if (vShader->Init())
    {
        vShadersList[vShader->GetShaderFileName()] = vShader;
        return true;
    }
    else
    {
        return false;
    }
}

bool ShaderManager::CreatePixelShader(LPCWSTR shaderFile)
{
    PixelShader* pShader = new PixelShader(GraphicsEngine::GetInstance(), shaderFile);

    if (pShader->Init())
    {
        pShadersList[pShader->GetShaderFileName()] = pShader;
        return true;
    }
    else
    {
        return false;
    }
}

bool ShaderManager::CreateShaderProgram(LPCWSTR shaderType, LPCWSTR vShaderName, LPCWSTR pShaderName)
{
    ShaderProgram newShader;
    newShader.shaderType = shaderType;
    newShader.vShader = vShadersList[vShaderName];
    newShader.pShader = pShadersList[pShaderName];

    int indexForNew = (int)shaderProgramsList.size();
    for (int i = 0; i < shaderProgramsList.size(); i++)
    {
        ShaderProgram& shaderInList = shaderProgramsList[i];

        if (shaderInList.pShader == newShader.pShader)
        {
            indexForNew = i + 1;
            break;
        }
    }
    shaderProgramsList.insert(shaderProgramsList.begin() + indexForNew, newShader);

	return true;
}

std::vector<ShaderManager::ShaderProgram> ShaderManager::GetShaderProgramsList()
{
	return shaderProgramsList;
}