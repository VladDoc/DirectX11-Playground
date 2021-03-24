#include "input.h"

#include "util.h"
static GameState* data;
static Controls* controls;
static RenderContext* context;

void initInput(RenderContext& r, GameState& g, Controls& c)
{
	context = &r;
	data = &g;
	controls = &c;
}

#include "render.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include <d3d11.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (context->g_pd3dDevice && wParam != SIZE_MINIMIZED)
		{
			data->screenW = (UINT)LOWORD(lParam);
			data->screenH = (UINT)HIWORD(lParam);
			context->CleanupRenderTarget();
			context->g_pSwapChain->ResizeBuffers(0, data->screenW, data->screenH, DXGI_FORMAT_UNKNOWN, 0);
			context->CreateRenderTarget(*data);
		}
		return 0;
		break;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
		break;
	case WM_KEYUP:
		if (wParam == 'F') {
			SetWindowText(hWnd, L"Respects");
		}
		if (wParam == 'W') {
			controls->forward = false;
		}
		if (wParam == 'S') {
			controls->backward = false;
		}
		if (wParam == 'A') {
			controls->strafeLeft = false;
		}
		if (wParam == 'D') {
			controls->strafeRight = false;
		}
		if (wParam == VK_LEFT) {
			controls->left = false;
		}
		if (wParam == VK_RIGHT) {
			controls->right = false;
		}
		if (wParam == VK_UP) {
			controls->lookup = false;
		}
		if (wParam == VK_DOWN) {
			controls->lookdown = false;
		}
		if (wParam == 'E') {
			controls->rollLeft = false;
		}
		if (wParam == 'Q') {
			controls->rollRight = false;
		}
		if (wParam == VK_F4) {
			controls->vsync = !controls->vsync;
		}
		if (wParam == VK_F3) {
			controls->imgui = !controls->imgui;
		}
		if (wParam == VK_F2) {
			context->shader.reload(*context);
		}
		break;
	case WM_KEYDOWN:
		if (wParam == 'W') {
			controls->forward = true;
		}
		if (wParam == 'S') {
			controls->backward = true;
		}
		if (wParam == 'A') {
			controls->strafeLeft = true;
		}
		if (wParam == 'D') {
			controls->strafeRight = true;
		}
		if (wParam == VK_LEFT) {
			controls->left = true;
		}
		if (wParam == VK_RIGHT) {
			controls->right = true;
		}
		if (wParam == VK_UP) {
			controls->lookup = true;
		}
		if (wParam == VK_DOWN) {
			controls->lookdown = true;
		}
		if (wParam == 'E') {
			controls->rollLeft = true;
		}
		if (wParam == 'Q') {
			controls->rollRight = true;
		}
		if (wParam == VK_ESCAPE) {
			data->done = true;
		}
		break;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void doActions(Controls& c, GameState& g)
{
	if (c.forward) {
		g.camera.moveForward(util::interpolateValue(g.frametime, Constants::walkSpeed));
	}
	if (c.backward) {
		g.camera.moveForward(-util::interpolateValue(g.frametime, Constants::walkSpeed));
	}
	if (c.strafeLeft) {
		g.camera.moveRight(-util::interpolateValue(g.frametime, Constants::walkSpeed));
	}
	if (c.strafeRight) {
		g.camera.moveRight(util::interpolateValue(g.frametime, Constants::walkSpeed));
	}
	if (c.left) {
		g.camera.rotateYaw(-util::interpolateValue(g.frametime, Constants::rotatingSpeed));
	}
	if (c.right) {
		g.camera.rotateYaw(util::interpolateValue(g.frametime, Constants::rotatingSpeed));
	}

	if (c.lookup) {
		g.camera.rotatePitch(util::interpolateValue(g.frametime, Constants::rotatingSpeed));
	}
	if (c.lookdown) {
		g.camera.rotatePitch(-util::interpolateValue(g.frametime, Constants::rotatingSpeed));
	}

	if (c.rollLeft) {
		g.camera.rotateRoll(-util::interpolateValue(g.frametime, Constants::rotatingSpeed));
	}
	if (c.rollRight) {
		g.camera.rotateRoll(util::interpolateValue(g.frametime, Constants::rotatingSpeed));
	}
}