#pragma once

#include "NonUglyMath.h"

#include <d3d11.h>

struct Vertex {
	float4 position;
	float2 uv;
	float3 normal;
	
	static const D3D11_INPUT_ELEMENT_DESC input_desc[3];
};