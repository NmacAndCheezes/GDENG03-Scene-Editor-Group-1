#include "SwapChain.h"
#include "../GameEngine/Graphics/GraphicsEngine.h"


SwapChain::SwapChain(GraphicsEngine* gfx) : gfx (gfx)
{

}

SwapChain::~SwapChain()
{

}

bool SwapChain::Init(HWND hWnd, UINT width, UINT height)
{
	ID3D11Device* device = gfx->GetDevice();


	DXGI_SWAP_CHAIN_DESC desc = {};
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hWnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;
	 
	// create swap chain
	HRESULT res = gfx->GetFactory()->CreateSwapChain(device, &desc, &swapChain);
	if (FAILED(res)) return false;

	// retrieve back buffer, then create render target view
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (FAILED(res)) return false;

	res = device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView); 
	backBuffer.Get()->Release();
	if (FAILED(res)) return false;

	// create depth stensil state 
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};  
	dsDesc.DepthEnable = TRUE;  
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;  
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;  
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState;  
	res = device->CreateDepthStencilState(&dsDesc, &dsState); 
	if (FAILED(res)) return false;

	// create depth stensil texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> dsTexture; 
	D3D11_TEXTURE2D_DESC descDepth = {};  
	descDepth.Width = width;   
	descDepth.Height = height;  
	descDepth.MipLevels = 1u;  
	descDepth.ArraySize = 1u;  
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;  
	descDepth.SampleDesc.Count = 1u;  
	descDepth.SampleDesc.Quality = 0u;  
	descDepth.Usage = D3D11_USAGE_DEFAULT;  
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;  
	res = device->CreateTexture2D(&descDepth, nullptr, &dsTexture); 
	if (FAILED(res)) return false; 

	// create depth stencil view using texture
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};   
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;  
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;  
	dsvDesc.Texture2D.MipSlice = 0u;  
	res = device->CreateDepthStencilView(dsTexture.Get(), &dsvDesc, &depthStencilView); 
	if (FAILED(res)) return false;


	gfx->GetDeviceContext()->OMSetDepthStencilState(dsState.Get(), 1u); 
	gfx->GetDeviceContext()->OMSetRenderTargets(1u, renderTargetView.GetAddressOf(), depthStencilView.Get());

	dsState.Get()->Release();
	dsTexture.Get()->Release();

	return true;
}

bool SwapChain::Release()
{
	if (swapChain.Get()) swapChain.Get()->Release();
	if (renderTargetView.Get()) renderTargetView.Get()->Release();
	if (depthStencilView.Get()) depthStencilView.Get()->Release();

	delete this;

	return true;
}

bool SwapChain::ClearBuffer(float r, float g, float b)
{
	if (renderTargetView.Get() == nullptr)
	{
		return false;
	}

	const float color[] = { r, g, b, 1.0f };
	gfx->GetDeviceContext()->ClearRenderTargetView(renderTargetView.Get(), color);
	gfx->GetDeviceContext()->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

	return true;
}

bool SwapChain::Present(bool vSync)
{
	swapChain->Present(vSync, 0);

	return true;
}
