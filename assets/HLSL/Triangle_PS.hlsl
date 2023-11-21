struct VertexOut
{
	float4 posH : SV_POSITION;
};

cbuffer cb : register(b0)
{
	float4 color;
}

SamplerState samplerState : register(s0);
Texture2D texture2d : register(t0);

float4 PS(VertexOut pIn) : SV_Target
{
	return texture2d.Sample(samplerState, float2(1,1));
	//return color;
}
