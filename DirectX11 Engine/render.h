#pragma once

struct RenderContext;
struct GameState;
struct Controls;

namespace Render {
	void init(RenderContext&, GameState&, Controls&);

	void draw(RenderContext&, GameState&, Controls&);

	void close(RenderContext&, GameState&, Controls&);
}


void loadTextures(RenderContext& context, const GameState& data);
void setSamplers(RenderContext& context, const GameState& data);