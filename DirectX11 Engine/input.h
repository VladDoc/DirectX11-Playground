#pragma once

#include "gamestate.h"
#include "controls.h"
#include "render_context.h"

#include <Windows.h>

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void initInput(RenderContext& r, GameState& g, Controls& c);

void doActions(Controls&, GameState&);
