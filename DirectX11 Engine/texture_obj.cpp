#include "texture_obj.h"

#include "util.h"

#include "wic/TextureLoader.h"

#include "render_context.h"

static int counter = 0;

void PSSetShaderResourcesProxy(ID3D11DeviceContext* context, 
							   const std::vector<ID3D11ShaderResourceView*>& shaderResourceViews)
{
	context->PSSetShaderResources(counter, shaderResourceViews.size(), shaderResourceViews.data());
	counter += shaderResourceViews.size();
}

void TextureObj::FrameEnd()
{
	counter = 0;
}

void TextureObj::bind(RenderContext& context, const GameState& data) const
{
	PSSetShaderResourcesProxy(context.g_pd3dDeviceContext, this->shaderResourceViews);
}

void TextureObj::init(RenderContext& context, const GameState& data)
{
	textures.push_back(nullptr);
	shaderResourceViews.push_back(nullptr);

	auto path = util::to_wchar(path_.c_str());

	HRESULT hr = CreateWICTextureFromFile(context.g_pd3dDevice,
										  context.g_pd3dDeviceContext,
										  path.first.data(),
										  &textures.back(),
										  &shaderResourceViews.back());
	if (FAILED(hr)) {
		MessageBoxA(context.hwnd, (std::string("Unable to load texture ") + path_).c_str(),
					"Tough luck", MB_ICONERROR | MB_OK);
	}
}

void TextureObj::release()
{
	for (auto& a : textures) util::safeRelease(a);
	for (auto& a : shaderResourceViews) util::safeRelease(a);
}