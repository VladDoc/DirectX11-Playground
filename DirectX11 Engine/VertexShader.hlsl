struct PSInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

PSInput VSmain(uint id:SV_VERTEXID)
{
	PSInput output;

	output.pos.x = (float)(id / 2) * 4.0f - 1.0f;
	output.pos.y = (float)(id % 2) * 4.0f - 1.0f;
	output.pos.z = 0.0f;
	output.pos.w = 1.0f;

	output.uv.x = (float)(id / 2) * 2.0f;
	output.uv.y = 1.0f - (float)(id % 2) * 2.0f;

	output.normal = float3(1, 1, 1);

	return output;
}
