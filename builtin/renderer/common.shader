cbuffer cbPerObject : register(b0)
{
	matrix u_worldMatrix;
}

cbuffer cbView : register(b1)
{
	matrix g_view;  
}

cbuffer cbProj : register(b2)
{
	matrix g_proj;
}
