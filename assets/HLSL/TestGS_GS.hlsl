#include "Common.hlsli"

[maxvertexcount(6)]
void GS(triangle VertexOut input[3] : SV_POSITION, inout TriangleStream<VertexOut> output)
{
	VertexOut vertices[4];

	int width = 4;
	int height = 8;

	vertices[0].positionH = input[0].positionH;
	vertices[0].texcoord = float2(0.0f, 0.0f);
	vertices[1].positionH = input[0].positionH;
	vertices[1].texcoord = float2(1.0f, 0.0f);
	vertices[2].positionH = input[0].positionH;
	vertices[2].texcoord = float2(1.0f, 1.0f);
	vertices[3].positionH = input[0].positionH;
	vertices[3].texcoord = float2(0.0f, 1.0f);

	vertices[0].positionH.x = vertices[0].positionH.x - width / 2;
	vertices[0].positionH.y = vertices[0].positionH.y + height / 2;

	vertices[1].positionH.x = vertices[1].positionH.x + width / 2;
	vertices[1].positionH.y = vertices[1].positionH.y + height / 2;

	vertices[2].positionH.x = vertices[2].positionH.x + width / 2;
	vertices[2].positionH.y = vertices[2].positionH.y - height / 2;

	vertices[3].positionH.x = vertices[3].positionH.x - width / 2;
	vertices[3].positionH.y = vertices[3].positionH.y - height / 2;

	output.Append(vertices[0]);
	output.Append(vertices[1]);
	output.Append(vertices[3]);

	output.RestartStrip();

	output.Append(vertices[1]);
	output.Append(vertices[2]);
	output.Append(vertices[3]);

	output.RestartStrip();

	//Vertex vertices[6];

	//for (uint i = 0; i < 3; i++)
	//{
	//	vertices[i] = input[i];
	//}
	//vertices[3].position = (input[0].position + input[1].position) / 2;
	//vertices[4].position = (input[1].position + input[2].position) / 2;
	//vertices[5].position = (input[0].position + input[2].position) / 2;
	//
	//output.Append(vertices[3]);
	//output.Append(vertices[1]);
	//output.Append(vertices[4]);

	//output.RestartStrip();

	//output.Append(vertices[0]);
	//output.Append(vertices[3]);
	//output.Append(vertices[5]);

	//output.RestartStrip();

	//output.Append(vertices[3]);
	//output.Append(vertices[4]);
	//output.Append(vertices[5]);

	//output.RestartStrip();

	//output.Append(vertices[5]);
	//output.Append(vertices[4]);
	//output.Append(vertices[2]);

	//output.RestartStrip();
}