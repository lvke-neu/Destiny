
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

cbuffer cbMaterialColor : register(b3)
{
	float4 g_ambientColor;
	float4 g_diffuseColor;
	float4 g_specularColor;
	float4 g_useColor;
}

cbuffer cbDirectLight : register(b4)
{
	float4 g_directLightDirection;
	matrix g_directLightView;
	matrix g_directLightProj;
	matrix g_T;
}

struct VertexIn
{
	float3 positionL : POSITION;
	float3 normalL : NORMAL;
	float2 texcoord : TEXCOORD;
};
struct VertexOut
{
	float3 positionL : POSITIONL;
	float4 positionH : SV_POSITION;
	float4 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 texcoord : TEXCOORD;
	float4 shadowMapPositionH : TEXCOORD1;
};

struct ParticleVertexIn
{
	float3 positionL : POSITION;
	float4 color : COLOR;
};
struct ParticleVertexOut
{
	float4 positionH : POSITIONL;
	float4 color : COLOR;
};


Texture2D g_ambientTexture : register(t0);
SamplerState g_ambientSampler : register(s0);
Texture2D g_diffuseTexture : register(t1);
SamplerState g_diffuseSampler : register(s1);
Texture2D g_specularTexture : register(t2);
SamplerState g_specularSampler : register(s2);

Texture2D g_shadowMapTexture : register(t3);
SamplerComparisonState g_shadowMapSampler : register(s3);