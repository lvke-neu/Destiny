cbuffer cbView : register(b0)
{
	matrix g_view;  
	float4 g_eyePos;
}

cbuffer cbProj : register(b1)
{
	matrix g_proj;
}

cbuffer cbWorld : register(b2)
{
	matrix g_world;
	matrix g_worldInvTranspose;
}
