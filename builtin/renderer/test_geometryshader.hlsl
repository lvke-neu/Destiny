#include "common.hlsl"

struct VertexIn
{
	float3 positionL : POSITION;
	float4 color	 : COLOR;
};

struct VertexOut
{
	float4 positionH : SV_POSITION;
	float4 color	 : COLOR;
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.positionH = float4(vIn.positionL, 1.0f);
	vOut.color = vIn.color;
	return vOut;
}

[maxvertexcount(9)]
void GS(triangle VertexOut input[3], inout TriangleStream<VertexOut> output)
{
	VertexOut vertices[6];

	vertices[0] = input[0];
	vertices[1] = input[1];
	vertices[2] = input[2];
	vertices[3].positionH = (vertices[0].positionH + vertices[1].positionH ) / 2.0f;
	vertices[3].color = (vertices[0].color + vertices[1].color) / 2.0f;
	vertices[4].positionH = (vertices[1].positionH + vertices[2].positionH) / 2.0f;
	vertices[4].color = (vertices[1].color + vertices[2].color) / 2.0f;
	vertices[5].positionH = (vertices[0].positionH + vertices[2].positionH) / 2.0f;
	vertices[5].color = (vertices[0].color + vertices[2].color) / 2.0f;

	output.Append(vertices[0]);
	output.Append(vertices[3]);
	output.Append(vertices[5]);
	output.RestartStrip();

	output.Append(vertices[3]);
	output.Append(vertices[1]);
	output.Append(vertices[4]);
	output.RestartStrip();

	//output.Append(vertices[5]);
	//output.Append(vertices[3]);
	//output.Append(vertices[4]);
	//output.RestartStrip();

	output.Append(vertices[5]);
	output.Append(vertices[4]);
	output.Append(vertices[2]);
	output.RestartStrip();

}

float4 PS(VertexOut pIn) : SV_Target
{
	return pIn.color;
}

