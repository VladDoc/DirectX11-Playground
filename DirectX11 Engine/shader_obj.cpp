#include "shader_obj.h"

#include <array>

#include <d3d11.h>
#include <D3Dcompiler.h>
#pragma comment(lib,"D3dcompiler.lib")

#include "util.h"

#include "render_context.h"

#include "vertex.h"


ShaderObj::~ShaderObj()
{
	this->release();
}

void ShaderObj::bind(RenderContext& context, const GameState& data) const
{
	context.g_pd3dDeviceContext->IASetInputLayout(inputLayout.get());

	context.g_pd3dDeviceContext->VSSetShader(vertexShader.get(), NULL, 0);
	context.g_pd3dDeviceContext->PSSetShader(pixelShader.get(), NULL, 0);
}

void ShaderObj::release()
{
}

void ShaderObj::reload(RenderContext& context)
{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#endif
		LPCSTR profileVS = (context.g_pd3dDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "vs_5_0" : "vs_4_0";
		LPCSTR profilePS = (context.g_pd3dDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";
		const D3D_SHADER_MACRO defines[] =
		{
			NULL, NULL
		};

		HRESULT hr;
		ID3DBlob* errorBlob = nullptr;
		ID3DBlob* oldBlob = vsBlob.get();

		auto path = util::to_wchar(vertexPath.c_str());

		hr = D3DCompileFromFile(path.first.data(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
								"VSmain", profileVS, flags, NULL, &vsBlob, &errorBlob);

		if (FAILED(hr))
		{

			FILE* errors;
			fopen_s(&errors, "errors.txt", "w");
			fprintf(errors, "Failed compiling vertex shader %08X\n", hr);
			fclose(errors);

			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				MessageBoxA(context.hwnd, (char*)errorBlob->GetBufferPointer(), "Shader compilation error", MB_ICONERROR | MB_OK);
				errorBlob->Release();
			}

			if (!vsBlob) {
				vsBlob.reset(oldBlob);
			}
		}


		oldBlob = psBlob.get();

		path = util::to_wchar(pixelPath.c_str());

		hr = D3DCompileFromFile(path.first.data(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
								"PSmain", profilePS, flags, NULL, &psBlob, &errorBlob);

		if (FAILED(hr))
		{

			FILE* errors;
			fopen_s(&errors, "errors.txt", "w");
			fprintf(errors, "Failed compiling pixel shader %08X\n", hr);

			fclose(errors);
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				MessageBoxA(context.hwnd, (char*)errorBlob->GetBufferPointer(), "Shader compilation error", MB_ICONERROR | MB_OK);
				errorBlob->Release();
			}

			if (!psBlob) {
				psBlob.reset(oldBlob);
			}
		}

		context.g_pd3dDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &vertexShader);
		context.g_pd3dDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &pixelShader);


		context.g_pd3dDevice->CreateInputLayout(Vertex::input_desc, std::size(Vertex::input_desc),
												vsBlob->GetBufferPointer(),
												vsBlob->GetBufferSize(), &inputLayout);

		OutputDebugStringA("Shader compiled successfully\n");
}