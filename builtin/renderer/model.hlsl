#include "common.hlsl"

cbuffer cbPerObject : register(b0)
{
	matrix u_worldMatrix;
    uint c_has_c_ambient;
    uint c_has_c_diffuse;
    uint c_has_t_ambient;
    uint c_has_t_diffuse;
    float4  c_ambient;
    float4  c_diffuse;
}

struct VertexIn
{
	float3 positionL : POSITION;
	float3 normal	 : NORMAL;
	float2 texcoord  : TEXCOORD;
};


struct VertexOut
{
	float4 positionH : SV_POSITION;
	float3 normal	 : NORMAL;
	float2 texcoord  : TEXCOORD;
};



VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.positionH = mul(mul(mul(float4(vIn.positionL, 1.0f), u_worldMatrix), g_view), g_proj);
	vOut.normal = vIn.normal;
	vOut.texcoord = vIn.texcoord;
	return vOut;
}

Texture2D t_ambient : register(t0);
Texture2D t_diffuse : register(t1);
SamplerState s_sampler : register(s0);

float4 PS(VertexOut pIn) : SV_Target
{
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	if(c_has_c_ambient && c_has_t_ambient)
    {
        ambient = c_ambient * t_ambient.Sample(s_sampler, pIn.texcoord);
    }
	else
    {
        if (c_has_c_ambient)
        {
            ambient = c_ambient;
        }
        else if(c_has_t_ambient)
        {
            ambient = t_ambient.Sample(s_sampler, pIn.texcoord);

        }
    }
	
    if (c_has_c_diffuse && c_has_t_diffuse)
    {
        diffuse = c_diffuse * t_diffuse.Sample(s_sampler, pIn.texcoord);
    }
    else
    {
        if (c_has_c_diffuse)
        {
            diffuse = c_diffuse;
        }
        else if (c_has_t_diffuse)
        {
            diffuse = t_diffuse.Sample(s_sampler, pIn.texcoord);

        }
    }
    return ambient + diffuse;
}

