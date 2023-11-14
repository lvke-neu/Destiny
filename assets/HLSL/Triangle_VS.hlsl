struct VertexIn
{
	float3 posL : POSITION;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
};


VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = float4(vIn.posL, 1.0f);  
    return vOut;
}
