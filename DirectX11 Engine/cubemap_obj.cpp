#include "cubemap_obj.h"

#include "stb_image/stb_image.h"

#include "render_context.h"

void CubeMapObj::init(RenderContext& context, const GameState& data)
{
	// Load all individual images into memory.
	unsigned char* images[6];
	int desiredChannels = 4;
	int height[6];
	int width[6];
	int depth[6];
	bool loadedImages = true;

	images[0] = stbi_load(paths.posx.c_str(), &width[0], &height[0], &depth[0], desiredChannels);
	images[1] = stbi_load(paths.negx.c_str(), &width[1], &height[1], &depth[1], desiredChannels);
	images[2] = stbi_load(paths.posy.c_str(), &width[2], &height[2], &depth[2], desiredChannels);
	images[3] = stbi_load(paths.negy.c_str(), &width[3], &height[3], &depth[3], desiredChannels);
	images[4] = stbi_load(paths.posz.c_str(), &width[4], &height[4], &depth[4], desiredChannels);
	images[5] = stbi_load(paths.negz.c_str(), &width[5], &height[5], &depth[5], desiredChannels);

	for (auto& a : images) loadedImages &= (a != nullptr);

	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//Ensure we have the images we want to put in the cube map.
	if (loadedImages)
	{
		//D3DObjects to create
		ID3D11Texture2D* cubeTexture = nullptr;
		ID3D11ShaderResourceView* shaderResourceView = nullptr;

		//Description of each face
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = width[0];
		texDesc.Height = height[0];
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 6;
		texDesc.Format = format;
		texDesc.CPUAccessFlags = 0;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		//The Shader Resource view description
		D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
		SMViewDesc.Format = texDesc.Format;
		SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		SMViewDesc.TextureCube.MipLevels = texDesc.MipLevels;
		SMViewDesc.TextureCube.MostDetailedMip = 0;

		//Array to fill which we will use to point D3D at our loaded CPU images.
		D3D11_SUBRESOURCE_DATA pData[6];
		for (int i = 0; i < 6; i++)
		{
			//Pointer to the pixel data
			pData[i].pSysMem = images[i];
			//Line width in bytes
			pData[i].SysMemPitch = width[i] * desiredChannels;
			// This is only used for 3d textures.
			pData[i].SysMemSlicePitch = 0;
		}

		//Create the Texture Resource
		HRESULT hr = context.g_pd3dDevice->CreateTexture2D(&texDesc, &pData[0], &cubeTexture);
		if (FAILED(hr))
		{
			return;
		}

		//If we have created the texture resource for the six faces 
		//we create the Shader Resource View to use in our shaders.
		hr = context.g_pd3dDevice->CreateShaderResourceView(cubeTexture, &SMViewDesc, &shaderResourceView);
		if (FAILED(hr))
		{
			return;
		}
		textures.push_back({ cubeTexture });
		shaderResourceViews.push_back({ shaderResourceView });
	}
}