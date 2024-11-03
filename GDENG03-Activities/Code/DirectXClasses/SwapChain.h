#pragma once
#include <wrl.h>
#include <d3d11.h>


class GraphicsEngine;

class SwapChain 
{
public:
	SwapChain(GraphicsEngine* gfx);
	~SwapChain();

	bool Init(HWND hWnd, UINT width, UINT height);
	bool Release();

	bool ClearBuffer(float r, float g, float b);
	bool Present(bool vSync);


private:
	GraphicsEngine* gfx;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
};