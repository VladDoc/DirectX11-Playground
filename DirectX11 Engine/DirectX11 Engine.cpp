#define NOMINMAX
#include <Windows.h>

#include "render_context.h"
// Data
static RenderContext context;

#include "controls.h"
#include "gamestate.h"
#include "input.h"
// User Data
static GameState data{false, 800, 600};
static Controls controls;

#include "render.h"

// Main code
int CALLBACK WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{

	initInput(context, data, controls);

	Render::init(context, data, controls);

	data.camera.fov = 1.0f;
	data.camera.setAspect(data.screenW, data.screenH);

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (!data.done && msg.message != WM_QUIT)
	{
		auto start = GetTickCount64();
		// Poll and handle messages (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

		doActions(controls, data);

		Render::draw(context, data, controls);

		auto end = GetTickCount64();

		data.frametime = end - start;
		data.time += data.frametime / 1000.0f;
	}

	Render::close(context, data, controls);

	return 0;
}
