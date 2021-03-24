#pragma once

#include <string>
#include <vector>

#include "d3dobj.h"

#include "vertex.h"

#include "d3dptr.h"

struct VertexBufferObj : public D3DObj
{
	VertexBufferObj(const char* s) : obj_path(s) {}
	VertexBufferObj() = default;
	VertexBufferObj(VertexBufferObj&&) = default;
	VertexBufferObj(const VertexBufferObj&) = default;
	VertexBufferObj& operator=(const VertexBufferObj&) = default;
	VertexBufferObj& operator=(VertexBufferObj&&) = default;

	virtual void bind(RenderContext&, const GameState&) const override;
	virtual void init(RenderContext&, const GameState&) override;
	virtual void release() override;

	static void EndFrame();

protected:
	std::string obj_path;
	D3DPtr<ID3D11Buffer> vertex_buff;
};