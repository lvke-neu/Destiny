struct VertexIn
{
	float3 positionL : POSITION;
	float2 texcoord  : TEXCOORD;
};

struct VertexOut
{
	float4 positionH : SV_POSITION;
	float2 texcoord	 : TEXCOORD;
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.positionH = float4(vIn.positionL, 1.0f);
	vOut.texcoord = vIn.texcoord;
	return vOut;
}

Texture2D t_texture : register(t0);
SamplerState s_sampler : register(s0);
float4 PS(VertexOut pIn) : SV_Target
{
	return float4(1,0,0,1);
}

