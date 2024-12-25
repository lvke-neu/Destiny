#include "common.hlsl"

cbuffer cbPerObject : register(b0)
{
	matrix u_worldMatrix;
    matrix u_worldInvTransposeMatrix;
    uint c_has_c_ambient;
    uint c_has_c_diffuse;
    uint c_has_c_specular;
    uint c_has_t_ambient;
    uint c_has_t_diffuse;
    uint c_has_t_specular;
    float4  c_ambient;
    float4  c_diffuse;
    float4  c_specular;
}

struct VertexIn
{
	float3 positionL : POSITION;
	float3 normalL	 : NORMAL;
	float2 texcoord  : TEXCOORD;
};


struct VertexOut
{
	float4 positionH : SV_POSITION;
	float4 positionW : POSITION;
    float3 normalW   : NORMAL;
	float2 texcoord  : TEXCOORD;
};



VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.positionH = mul(mul(mul(float4(vIn.positionL, 1.0f), u_worldMatrix), g_view), g_proj);
    vOut.positionW = mul(float4(vIn.positionL, 1.0f), u_worldMatrix);
    vOut.normalW = mul(vIn.normalL, (float3x3) u_worldInvTransposeMatrix);
	vOut.texcoord = vIn.texcoord;
   
	return vOut;
}

Texture2D t_ambient : register(t0);
Texture2D t_diffuse : register(t1);
Texture2D t_specular : register(t2);
SamplerState s_sampler : register(s0);

float4 PS(VertexOut pIn) : SV_Target
{
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	[flatten]
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
	[flatten]
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
    [flatten]
    if (c_has_c_specular && c_has_t_specular)
    {
        specular = c_specular * t_specular.Sample(s_sampler, pIn.texcoord);
    }
    else
    {
        if (c_has_c_specular)
        {
            specular = c_specular;
        }
        else if (c_has_t_specular)
        {
            specular = t_specular.Sample(s_sampler, pIn.texcoord);

        }
    }
    
    float3 lightDir = float3(0.0f, -1.0f, 1.0f);
    float diffuseFactor = dot(normalize(-lightDir), normalize(pIn.normalW));
    
    float3 v = reflect(lightDir, pIn.normalW);
    float specularFactor = pow(max(dot(normalize(v), normalize(g_eyePosition - pIn.positionW.xyz)), 0.0f), 32.0f);
    
    return ambient + diffuseFactor * diffuse + specularFactor * specular;
}
