#pragma once

#include "d3dobj.h"

#include "constant_buffer.h"

#include <d3d11.h>

struct RenderContext;
struct GameState;

struct ConstBufObj : public D3DObj
{
	ID3D11Buffer* constbufHandle = nullptr;

	virtual void release() override;

	virtual void init(RenderContext& context, const GameState& data) override;

	virtual void bind(RenderContext& context, const GameState& data) const override;

	// const correctness is preserved
	const ConstantBuffer& get() const { return constbuf; }

	ConstantBuffer& get() { return constbuf; }

protected:
	// must update it in the bind function without breaking up the semantics, hence mutable 'thingy'
	mutable ConstantBuffer constbuf;
};