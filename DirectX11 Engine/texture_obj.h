#pragma once

#include "d3dobj.h"

#include <d3d11.h>

#include <vector>
#include <string>

struct TextureObj : public D3DObj
{

	TextureObj() = default;
	TextureObj(TextureObj&&) = default;
	TextureObj(const TextureObj&) = default;
	TextureObj& operator=(const TextureObj&) = default;
	TextureObj& operator=(TextureObj&&) = default;

	TextureObj(const char* path) : path_(path) {}
	virtual void bind(RenderContext&, const GameState&) const override;
	virtual void init(RenderContext&, const GameState&) override;
	virtual void release() override;

	std::string path_ = "";
	std::vector<ID3D11Resource*> textures;
	std::vector<ID3D11ShaderResourceView*> shaderResourceViews;

	static void FrameEnd();
};