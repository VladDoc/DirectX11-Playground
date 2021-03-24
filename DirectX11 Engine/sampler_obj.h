#pragma once

#include <d3d11.h>

#include "d3dobj.h"
#include "d3dptr.h"


struct SamplerObj : public D3DObj
{
	SamplerObj() = default;
	SamplerObj(const SamplerObj&) = default;
	SamplerObj(SamplerObj&&) = default;
	SamplerObj& operator=(const SamplerObj&) = default;
	SamplerObj& operator=(SamplerObj&&) = default;

	virtual void bind(RenderContext&, const GameState&) const override;
	virtual void init(RenderContext&, const GameState&) override;
	virtual void release() override;

	void initWithSamplerDesc(RenderContext&, const GameState&, const D3D11_SAMPLER_DESC&);

	D3DPtr<ID3D11SamplerState> sampler = nullptr;

	static void FrameEnd();
};