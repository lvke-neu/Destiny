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
};

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

        float4 skinnedPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float3 skinnedNormal = float3(0.0f, 0.0f, 0.0f);

        skinnedPos = mul(float4(vIn.positionL, 1.0f), BoneTransform);

        vOut.positionH = mul(mul(mul(skinnedPos, u_worldMatrix), g_shadowView), g_shadowProj);
    }
    else
    {
        vOut.positionH = mul(mul(mul(float4(vIn.positionL, 1.0f), u_worldMatrix), g_shadowView), g_shadowProj);
    }

    return vOut;
}

void PS(VertexOut pIn)
{

}
