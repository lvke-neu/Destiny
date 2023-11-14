struct VertexOut
{
	float4 posH : SV_POSITION;
};

float4 PS(VertexOut pIn) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
