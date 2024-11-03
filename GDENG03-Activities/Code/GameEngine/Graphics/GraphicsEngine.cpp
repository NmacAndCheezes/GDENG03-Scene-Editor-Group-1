#include "GraphicsEngine.h"
#include "ShaderManager.h"


GraphicsEngine* GraphicsEngine::sharedInstance = nullptr;

GraphicsEngine* GraphicsEngine::GetInstance()
{
    if (sharedInstance == nullptr)
    {
        sharedInstance = new GraphicsEngine();
    }

    return sharedInstance;
}

GraphicsEngine::GraphicsEngine() : d3d11Device(nullptr), d3d11Context(nullptr),
    dxgiDevice(nullptr), dxgiAdapter(nullptr), dxgiFactory(nullptr)
{

}

GraphicsEngine::~GraphicsEngine()
{

}

bool GraphicsEngine::Init()
{
    D3D_DRIVER_TYPE driverTypes[] = { 
         D3D_DRIVER_TYPE_HARDWARE, 
         D3D_DRIVER_TYPE_WARP, 
         D3D_DRIVER_TYPE_REFERENCE 
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes); 

    D3D_FEATURE_LEVEL featureLevels[] = { 
        D3D_FEATURE_LEVEL_11_0 
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels); 

    HRESULT res = 0; 

    for (UINT i = 0; i < numDriverTypes;) 
    {
        if (SUCCEEDED(res = D3D11CreateDevice(nullptr, driverTypes[i], nullptr, 0, 
            featureLevels, numFeatureLevels, D3D11_SDK_VERSION,  
            &d3d11Device, nullptr, &d3d11Context))) 
        {
            break;
        }

        i++;
    }

    if (FAILED(res)) return false;

    d3d11Device.Get()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
    dxgiDevice.Get()->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter); 
    dxgiAdapter.Get()->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

    return InitializeShaders();
}

bool GraphicsEngine::InitializeShaders()
{
    ShaderManager* sm = ShaderManager::GetInstance();
    Shader::SetShaderFolderDirectory(L"Code/Shaders/");

    std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
        //SEMANTIC NAME - SEMANTIC INDEX  -   FORMAT   -    INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
        {"POSITION",          0,    DXGI_FORMAT_R32G32B32_FLOAT,  0,           0,       D3D11_INPUT_PER_VERTEX_DATA,   0},
        {"COLOR",             0,    DXGI_FORMAT_R32G32B32_FLOAT,  0,    D3D11_APPEND_ALIGNED_ELEMENT,       D3D11_INPUT_PER_VERTEX_DATA,   0}
    };

    if (!sm->CreateVertexShader(L"VertexShader.cso", layout))
    {
        return false;
    }

    if (!sm->CreatePixelShader(L"PixelShader.cso"))
    {
        return false;
    }

    if (sm->CreateShaderProgram(L"DefaultShader", L"VertexShader.cso", L"PixelShader.cso"))
    {
        return false;
    }

    return true;
}

bool GraphicsEngine::Release()
{
    for (auto& sc : swapChainList)
    {
        if (sc != nullptr)
        {
            sc->Release();
        }
    }
    swapChainList.clear();
    swapChainList.shrink_to_fit();

    dxgiFactory.Get()->Release();
    dxgiAdapter.Get()->Release();
    dxgiDevice.Get()->Release();

    d3d11Context.Get()->Release();
    d3d11Device.Get()->Release();

    return true;
}

ID3D11Device* GraphicsEngine::GetDevice()
{
    return d3d11Device.Get();
}

ID3D11DeviceContext* GraphicsEngine::GetDeviceContext()
{
    return d3d11Context.Get();
}

IDXGIFactory* GraphicsEngine::GetFactory()
{
    return dxgiFactory.Get();
}

SwapChain* GraphicsEngine::CreateSwapChain(HWND hWnd, UINT width, UINT height)
{
    SwapChain* sc = new SwapChain(this);
    if (!sc->Init(hWnd, width, height))
    {
        return nullptr;
    }

    swapChainList.push_back(sc);
    return sc;
}

void GraphicsEngine::SetViewport(UINT width, UINT height)
{
    D3D11_VIEWPORT vp = {};
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;

    d3d11Context->RSSetViewports(1, &vp);
}