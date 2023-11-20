struct VertexOut
{
	float4 posH : SV_POSITION;
};

cbuffer testCB : register(b0)
{
	float4 color;
}

float4 PS(VertexOut pIn) : SV_Target
{
	return color;
}
