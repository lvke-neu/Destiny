cbuffer cbView : register(b1)
{
	matrix g_view;  
}

cbuffer cbProj : register(b2)
{
	matrix g_proj;
	float g_viewportWidth;
	float g_viewportHeight;
}
