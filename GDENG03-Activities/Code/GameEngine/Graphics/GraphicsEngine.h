#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include "../../DirectXClasses/SwapChain.h"


class GraphicsEngine
{
public:
	static GraphicsEngine* GetInstance();
	GraphicsEngine(const GraphicsEngine&) = delete;
	GraphicsEngine& operator=(const GraphicsEngine&) = delete;

	bool Init();
	bool Release();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGIFactory* GetFactory();
	SwapChain* CreateSwapChain(HWND hWnd, UINT width, UINT height);

	void SetViewport(UINT width, UINT height);


private:
	GraphicsEngine();
	~GraphicsEngine();

	bool InitializeShaders();


private:
	static GraphicsEngine* sharedInstance;

	Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d11Context;

	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
	Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;

	std::vector<SwapChain*> swapChainList;
};