#pragma once
#include <string>

#include "d3dobj.h"

struct GameScene : public D3DObj
{
	std::string path;

	GameScene(const char* obj_path) : path(obj_path) {}
	virtual void bind(RenderContext&, const GameState&) const;
	virtual void init(RenderContext&, const GameState&) {};
	virtual void release() {};
};