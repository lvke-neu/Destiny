#include "common.hlsl"

cbuffer cbPerObject : register(b0)
{
    matrix u_worldMatrix;
    matrix u_worldInvTransposeMatrix;
    float4x4 u_boneTransforms[256];
    float u_hasAnimation;
}

struct VertexIn
{
    float3 positionL : POSITION;
    float3 normalL   : NORMAL;
    float2 texcoord  : TEXCOORD;
    uint4 boneIds : BONEID0;
    float4 weights : WEIGHTS0;
    uint4 boneIds2 :BONEID1;
    float4 weights2 :WEIGHTS1;
};

struct VertexOut
{
    float4 positionH : SV_POSITION;
    float4 positionW : POSITION;
    float3 normalW   : NORMAL;
    float2 texcoord  : TEXCOORD;
    float4 shadowPosH : TEXCOORD1;
};


float3x3 Inverse3x3(float3x3 m) 
{
    float det00 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
    float det01 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
    float det02 = m[1][0] * m[2][1] - m[1][1] * m[2][0];


    float det = m[0][0] * det00 - m[0][1] * det01 + m[0][2] * det02;


    float invDet = 1.0f / max(det, 1e-6f);

    return float3x3(
        det00 * invDet,
        -(m[0][1] * m[2][2] - m[0][2] * m[2][1]) * invDet,
        (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet,

        -det01 * invDet,
        (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet,
        -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) * invDet,

        det02 * invDet,
        -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) * invDet,
        (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet
        );
}

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;

    if (u_hasAnimation > 0.5f)
    {
        matrix BoneTransform = u_boneTransforms[vIn.boneIds.x] * vIn.weights.x;
        BoneTransform += u_boneTransforms[vIn.boneIds.y] * vIn.weights.y;
        BoneTransform += u_boneTransforms[vIn.boneIds.z] * vIn.weights.z;
        BoneTransform += u_boneTransforms[vIn.boneIds.w] * vIn.weights.w;

        BoneTransform += u_boneTransforms[vIn.boneIds2.x] * vIn.weights2.x;
        BoneTransform += u_boneTransforms[vIn.boneIds2.y] * vIn.weights2.y;
        BoneTransform += u_boneTransforms[vIn.boneIds2.z] * vIn.weights2.z;
        BoneTransform += u_boneTransforms[vIn.boneIds2.w] * vIn.weights2.w;


        float4 skinnedPos = mul(float4(vIn.positionL, 1.0f), BoneTransform);


        float3x3 normalMatrix = transpose(Inverse3x3((float3x3)BoneTransform));


        float3 skinnedNormal = mul(vIn.normalL, normalMatrix);

        vOut.positionW = mul(skinnedPos, u_worldMatrix);
        vOut.positionH = mul(mul(mul(skinnedPos, u_worldMatrix), g_view), g_proj);


        float3x3 worldNormalMatrix = (float3x3) u_worldInvTransposeMatrix;
        vOut.normalW = mul(skinnedNormal, worldNormalMatrix);

        vOut.normalW = normalize(vOut.normalW);

        vOut.texcoord = vIn.texcoord;
        vOut.shadowPosH = mul(mul(mul(vOut.positionW, g_shadowView), g_shadowProj), T);
    }
    else
    {
        vOut.positionH = mul(mul(mul(float4(vIn.positionL, 1.0f), u_worldMatrix), g_view), g_proj);
        vOut.positionW = mul(float4(vIn.positionL, 1.0f), u_worldMatrix);
        vOut.normalW = mul(vIn.normalL, (float3x3) u_worldInvTransposeMatrix);
        vOut.texcoord = vIn.texcoord;
        vOut.shadowPosH = mul(mul(mul(vOut.positionW, g_shadowView), g_shadowProj), T);
    }

    return vOut;
}

Texture2D t_albedo : register(t0);
Texture2D t_normal : register(t1);
Texture2D t_metallic : register(t2);
Texture2D t_roughness : register(t3);
Texture2D t_ao : register(t4);
SamplerState s_sampler : register(s0);

GBuffer PS(VertexOut pIn)
{
	GBuffer gBuffer;
	gBuffer.albedo = t_albedo.Sample(s_sampler, pIn.texcoord);
	gBuffer.normal = t_normal.Sample(s_sampler, pIn.texcoord);
	float metallic = t_metallic.Sample(s_sampler, pIn.texcoord).x;
	float roughness = t_roughness.Sample(s_sampler, pIn.texcoord).x;
	float ao = t_ao.Sample(s_sampler, pIn.texcoord).x;
	gBuffer.mra = float4(metallic, roughness, ao, 1.0f);
	gBuffer.positionW = pIn.positionW;
	gBuffer.normalW = float4(pIn.normalW, 1.0f);
	gBuffer.texcoord = float4(pIn.texcoord, pIn.positionH.z, 1.0f);
	gBuffer.shadowPosH = pIn.shadowPosH;
	return gBuffer;
}
