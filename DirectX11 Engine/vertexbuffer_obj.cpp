#include "vertexbuffer_obj.h"

#include <d3d11.h>

#include "render_context.h"
#include "load_model.h"

void VertexBufferObj::bind(RenderContext& context, const GameState&) const
{
    constexpr UINT stride = sizeof(Vertex);
    constexpr UINT offset = 0;

    context.g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &vertex_buff, &stride, &offset);
}

void VertexBufferObj::init(RenderContext& context, const GameState&)
{

    auto model = LoadModel(this->obj_path.c_str());

    D3D11_BUFFER_DESC description{};
    description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    description.Usage = D3D11_USAGE_DEFAULT;
    description.ByteWidth = model.size() * util::elem_size(model);
    description.StructureByteStride = util::elem_size(model);

    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = model .data();

    const auto hr = context.g_pd3dDevice->CreateBuffer(&description, &subresource_data, &vertex_buff);

    if (FAILED(hr)) {
        MessageBoxA(NULL, (std::string("Couldn't load an obj file at ") + obj_path).c_str(),
                    "Failed to load an asset", MB_OK | MB_ICONERROR);
    }
}

void VertexBufferObj::release()
{

}

static void EndFrame()
{

}