#include "constbuf_obj.h"

#include "gamestate.h"
#include "render_context.h"

#include "util.h"
void ConstBufObj::release()
{
	util::safeRelease(constbufHandle);
}

void ConstBufObj::init(RenderContext& context, const GameState& data)
{
	D3D11_BUFFER_DESC constantBufferDescription = {};
	constantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDescription.ByteWidth = sizeof(ConstantBuffer);
	context.g_pd3dDevice->CreateBuffer(&constantBufferDescription, nullptr, &constbufHandle);
}

void ConstBufObj::bind(RenderContext& context, const GameState& data) const
{
	data.updateCB(constbuf);

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	context.g_pd3dDeviceContext->Map(constbufHandle, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	std::memcpy(mappedSubresource.pData, &constbuf, sizeof(constbuf));
	context.g_pd3dDeviceContext->Unmap(constbufHandle, 0);
	context.g_pd3dDeviceContext->PSSetConstantBuffers(0, 1, &constbufHandle);
}