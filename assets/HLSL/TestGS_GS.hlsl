struct Vertex
{
	float4 position : SV_POSITION;
};


[maxvertexcount(12)]
void GS(triangle Vertex input[3] : SV_POSITION, inout TriangleStream<Vertex> output)
{
	Vertex vertices[6];

	for (uint i = 0; i < 3; i++)
	{
		vertices[i] = input[i];
	}
	vertices[3].position = (input[0].position + input[1].position) / 2;
	vertices[4].position = (input[1].position + input[2].position) / 2;
	vertices[5].position = (input[0].position + input[2].position) / 2;
	
	output.Append(vertices[3]);
	output.Append(vertices[1]);
	output.Append(vertices[4]);

	output.RestartStrip();

	output.Append(vertices[0]);
	output.Append(vertices[3]);
	output.Append(vertices[5]);

	output.RestartStrip();

	output.Append(vertices[3]);
	output.Append(vertices[4]);
	output.Append(vertices[5]);

	output.RestartStrip();

	output.Append(vertices[5]);
	output.Append(vertices[4]);
	output.Append(vertices[2]);

	output.RestartStrip();
}