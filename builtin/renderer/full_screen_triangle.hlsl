struct VertexOut
{
	float4 positionH : SV_POSITION;
	float2 texcoord	 : TEXCOORD;
};

VertexOut VS(uint vertexID : SV_VertexID)
{
	VertexOut vOut;
	float2 grid = float2((vertexID << 1) & 2, vertexID & 2);
	float2 xy = grid * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f);
	vOut.positionH = float4(xy, 1.0f, 1.0f);
	vOut.texcoord.x = (xy.x + 1.0f) * 0.5f;
	vOut.texcoord.y = (1.0f - xy.y) * 0.5f;
	return vOut;
}


Texture2D t_texture : register(t0);
SamplerState s_sampler : register(s0);
float4 PS(VertexOut pIn) : SV_Target
{
	return float4(1,0,0,1);
}

