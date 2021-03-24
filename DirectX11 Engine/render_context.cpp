#include "render_context.h"

#include "util.h"

#include "constbuf_obj.h"

void RenderContext::release()
{
	util::safeRelease(g_mainRenderTargetView);
	util::safeRelease(g_pSwapChain);
	util::safeRelease(g_pd3dDeviceContext);
	util::safeRelease(g_pd3dDevice);
}

RenderContext::~RenderContext()
{
	release();
}

bool RenderContext::CreateDeviceD3D(const GameState& data)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = data.screenW;
	sd.BufferDesc.Height = data.screenH;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;



	UINT createDeviceFlags = 0;
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0,};
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags,
									  featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain,
									  &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK) {
		return false;
	}

	D3DPtr<ID3D11RasterizerState> state;
	D3D11_RASTERIZER_DESC rasterizerDesc{};

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.MultisampleEnable = Constants::multisample > 1;
	rasterizerDesc.AntialiasedLineEnable = false;

	const auto hr = this->g_pd3dDevice->CreateRasterizerState(&rasterizerDesc, &state);
	if (FAILED(hr)) {
		MessageBoxA(NULL, "Failed to initialize rasterizer",
					"Graphics could not initilize", MB_OK | MB_ICONERROR);
	} else {
		this->g_pd3dDeviceContext->RSSetState(state.get());
	}
	this->CreateRenderTarget(data);
	return true;
}

void RenderContext::CreateRenderTarget(const GameState& data)
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);

	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(data.screenW);
	viewport.Height = static_cast<float>(data.screenH);

	g_pd3dDeviceContext->RSSetViewports(1, &viewport);

	pBackBuffer->Release();
}

void RenderContext::CleanupRenderTarget()
{
	util::safeRelease(g_mainRenderTargetView);
}