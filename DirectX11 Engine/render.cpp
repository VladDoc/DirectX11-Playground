#include "render.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include <string>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <D3Dcompiler.h>
#pragma comment(lib,"D3dcompiler.lib")
#include <comdef.h>

#include "render_context.h"
#include "gamestate.h"
#include "controls.h"
#include "constant_buffer.h"

#include "wic/TextureLoader.h"

#include "stb_image/stb_image.h"

#include "input.h"


void Render::init(RenderContext& context, GameState& data, Controls& controls)
{
	// Create application window
	context.wc = {sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL,  L"V. Doc Engine", NULL};
	::RegisterClassEx(&context.wc);
	context.hwnd = ::CreateWindow(context.wc.lpszClassName, L"V. Doc Engine", WS_OVERLAPPEDWINDOW,
									100, 100, data.screenW, data.screenH, NULL, NULL, context.wc.hInstance, NULL);

	// Initialize Direct3D
	if (!context.CreateDeviceD3D(data))
	{
		context.release();
		::UnregisterClass(context.wc.lpszClassName, context.wc.hInstance);
		exit(1);
	}

	// Show the window
	::ShowWindow(context.hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(context.hwnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); //(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(context.hwnd);
	ImGui_ImplDX11_Init(context.g_pd3dDevice, context.g_pd3dDeviceContext);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	context.constbuf.init(context, data);
	context.shader.init(context, data);
	loadTextures(context, data);
	context.cubemap.init(context, data);
	setSamplers(context, data);
}

void loadTextures(RenderContext& context, const GameState& data)
{
	context.sviborg.init(context, data);
	
	context.moon.init(context, data);
	
	context.floor.init(context, data);
}

void setSamplers(RenderContext& context, const GameState& data)
{
	context.sampler.init(context, data);
}


void Render::draw(RenderContext& context, GameState& data, Controls& controls)
{
	// Start the Dear ImGui frame
	if (controls.imgui) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	// vertex buffer bind
	context.g_pd3dDeviceContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	context.g_pd3dDeviceContext->IASetIndexBuffer(NULL, (DXGI_FORMAT)0, 0);

	context.shader.bind(context, data);

	// sampler bind
	context.sampler.bind(context, data);

	context.sviborg.bind(context, data);
	context.moon.bind(context, data);
	context.floor.bind(context, data);

	context.cubemap.bind(context, data);

	context.constbuf.bind(context, data);
	
	// Drawing bind
	context.g_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context.g_pd3dDeviceContext->Draw(3, 0);
	
	if (controls.imgui) {
		data.renderImGui();

		ImGui::Render();
		context.g_pd3dDeviceContext->OMSetRenderTargets(1, &context.g_mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	TextureObj::FrameEnd();
	SamplerObj::FrameEnd();

	context.g_pSwapChain->Present(controls.vsync, 0);
}

void Render::close(RenderContext& context, GameState&, Controls&)
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	context.release();

	::DestroyWindow(context.hwnd);
	::UnregisterClass(context.wc.lpszClassName, context.wc.hInstance);
}
