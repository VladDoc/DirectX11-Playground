#pragma once

struct RenderContext;
struct GameState;

struct D3DObj
{
	virtual void bind(RenderContext&, const GameState&) const = 0;
	virtual void init(RenderContext&, const GameState&) {};
	virtual void release() {};
	virtual ~D3DObj() {}
};