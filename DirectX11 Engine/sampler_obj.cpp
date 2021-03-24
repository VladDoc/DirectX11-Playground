#include "sampler_obj.h"

#include "util.h"

#include "render_context.h"

static int counter = 0;

void SamplerObj::FrameEnd()
{
	counter = 0;
}

void SamplerObj::bind(RenderContext& context, const GameState& data) const
{
	context.g_pd3dDeviceContext->PSSetSamplers(counter++, 1, &sampler);
}

void SamplerObj::initWithSamplerDesc(RenderContext& context, const GameState& data, 
									 const D3D11_SAMPLER_DESC& samplerDesc)
{
	HRESULT hr = context.g_pd3dDevice->CreateSamplerState(&samplerDesc, &sampler);
	if (FAILED(hr)) {
		MessageBoxA(context.hwnd, "Unable to set sampler", "Tough luck", MB_ICONERROR | MB_OK);
	}
}

void SamplerObj::init(RenderContext& context, const GameState& data)
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	HRESULT hr = context.g_pd3dDevice->CreateSamplerState(&samplerDesc, &sampler);
	if (FAILED(hr)) {
		MessageBoxA(context.hwnd, "Unable to set sampler", "Tough luck", MB_ICONERROR | MB_OK);
	}
}

void SamplerObj::release()
{
}