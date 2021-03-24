#pragma once
#include <Windows.h>

#include <d3d11.h>

#include <vector>

#include "gamestate.h"
#include "shader_obj.h"
#include "texture_obj.h"
#include "constbuf_obj.h"
#include "cubemap_obj.h"
#include "sampler_obj.h"

struct RenderContext
{
	ID3D11Device*            g_pd3dDevice = nullptr;
	ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
	IDXGISwapChain*          g_pSwapChain = nullptr;
	ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

	ShaderObj shader{"PixelShader.hlsl", "VertexShader.hlsl"};

	SamplerObj sampler;

	TextureObj sviborg{"assets/sviborg.jpg"};
	TextureObj moon{"assets/moon.jpg"};
	TextureObj floor{"assets/floor.jpg"};

	CubeMapObj cubemap{"assets/posx.jpg", "assets/negx.jpg",
					   "assets/posy.jpg", "assets/negy.jpg",
					   "assets/posz.jpg", "assets/negz.jpg"};

	ConstBufObj constbuf;

	HWND hwnd;
	WNDCLASSEX wc;

	void release();

	~RenderContext();

	bool CreateDeviceD3D(const GameState& data);

	void CreateRenderTarget(const GameState& data);

	void CleanupRenderTarget();
};