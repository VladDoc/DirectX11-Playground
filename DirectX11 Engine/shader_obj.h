#pragma once

#include "d3dobj.h"

#include <d3d11.h>

#include <string>

#include "d3dptr.h"

struct ShaderObj : public D3DObj
{
	std::string pixelPath;
	std::string vertexPath;
	
	D3DPtr<ID3D11VertexShader> vertexShader = nullptr;
	D3DPtr<ID3D11PixelShader> pixelShader = nullptr;
	D3DPtr<ID3D11InputLayout> inputLayout = nullptr;

	D3DPtr<ID3DBlob> vsBlob = nullptr;
	D3DPtr<ID3DBlob> psBlob = nullptr;
	
	ShaderObj() = default;
	ShaderObj(const ShaderObj&) = default;
	ShaderObj(ShaderObj&&) = default;
	ShaderObj& operator=(const ShaderObj&) = default;
	ShaderObj& operator=(ShaderObj&&) = default;

	ShaderObj(const char* pixpath, const char* verpath) : pixelPath(pixpath), vertexPath(verpath) {}
	~ShaderObj();

	virtual void release() override;

	virtual void init(RenderContext& context, const GameState& data) override
	{
		this->reload(context);
	}

	virtual void bind(RenderContext&, const GameState&) const override;

	void reload(RenderContext&);
};