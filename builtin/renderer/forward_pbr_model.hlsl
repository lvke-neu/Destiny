#include "common.hlsl"

cbuffer cbPerObject : register(b0)
{
    matrix u_worldMatrix;
    matrix u_worldInvTransposeMatrix;
    float4x4 u_boneTransforms[256];
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

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    

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

    float3x3 normalMatrix = transpose((float3x3)BoneTransform);

    // 变换法线
    skinnedNormal = mul(vIn.normalL, normalMatrix);
    skinnedNormal = normalize(skinnedNormal); // 归一化以防止精度问题


    vOut.positionH = mul(mul(mul(skinnedPos, u_worldMatrix), g_view), g_proj);
    vOut.positionW = mul(skinnedPos, u_worldMatrix);
    vOut.normalW = mul(skinnedNormal, (float3x3) u_worldInvTransposeMatrix);
    vOut.texcoord = vIn.texcoord;
    vOut.shadowPosH = mul(mul(mul(vOut.positionW, g_shadowView), g_shadowProj), T);

    return vOut;
}

Texture2D t_albedo : register(t0);
Texture2D t_normal : register(t1);
Texture2D t_metallic : register(t2);
Texture2D t_roughness : register(t3);
Texture2D t_ao : register(t4);
TextureCube t_irradianceMap : register(t5); // 新增：辐照度贴图
TextureCube t_prefilterMap : register(t6);  // 新增：预过滤环境贴图
Texture2D t_brdfLUT : register(t7);         // 新增：BRDF查找表
SamplerState s_sampler : register(s0);
SamplerState s_cubeSampler : register(s1); // 新增：立方体贴图采样器

// 新增：考虑粗糙度的菲涅尔方程
float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max(float3(1.0f - roughness, 1.0f - roughness, 1.0f - roughness), F0) - F0) * pow(1.0f - cosTheta, 5.0f);
}

float4 PS(VertexOut pIn) : SV_Target
{
    float3 albedo = pow(t_albedo.Sample(s_sampler, pIn.texcoord).xyz, float3(2.2f, 2.2f, 2.2f));
    float metallic = t_metallic.Sample(s_sampler, pIn.texcoord).x;
    float roughness = t_roughness.Sample(s_sampler, pIn.texcoord).x;
    float ao = t_ao.Sample(s_sampler, pIn.texcoord).x;

    // 从法线贴图获取法线
    float3 tangentNormal = t_normal.Sample(s_sampler, pIn.texcoord).xyz * 2.0f - 1.0f;
    float3 Q1 = ddx(pIn.positionW.xyz);
    float3 Q2 = ddy(pIn.positionW.xyz);
    float2 st1 = ddx(pIn.texcoord);
    float2 st2 = ddy(pIn.texcoord);

    float3 N_ = normalize(pIn.normalW);
    float3 T = normalize(Q1 * st2.y - Q2 * st1.y);
    float3 B = -normalize(cross(N_, T));

    float3x3 TBN = float3x3(T, B, N_);

    float3 N = normalize(mul(tangentNormal, TBN));
    float3 V = normalize(g_eyePosition.xyz - pIn.positionW.xyz);
    float3 R = reflect(-V, N); // 新增：反射向量

    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, albedo, metallic);

    float3 Lo = float3(0.0f, 0.0f, 0.0f);

    // 阴影计算
    float shadow = calculateShadow(pIn.shadowPosH);

    // 直接光照
    for (int i = 0; i < g_directionLightCount; i++)
    {
        float3 L = normalize(-g_directionLights[i].lightDirection);
        float3 H = normalize(V + L);

        float3 radiance = g_directionLights[i].lightColor.xyz * g_directionLights[i].lightIntensity;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        float3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0);

        float3 nominator = NDF * G * F;
        float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.001f;
        float3 specular = nominator / denominator;

        float3 kS = F;
        float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
        kD *= 1.0f - metallic;

        float NdotL = max(dot(N, L), 0.0f);

        Lo += (kD * albedo / PI + specular) * radiance * NdotL * shadow;
    }

    for (int j = 0; j < g_pointLightCount; j++)
    {
        float3 L = normalize(g_pointLights[j].lightPosition - pIn.positionW.xyz);
        float3 H = normalize(V + L);

        float distance = length(g_pointLights[j].lightPosition - pIn.positionW.xyz);
        float attenuation = 1.0 / (distance * distance);
        float3 radiance = g_pointLights[j].lightColor.xyz * g_pointLights[j].lightIntensity * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        float3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0);

        float3 nominator = NDF * G * F;
        float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.001f;
        float3 specular = nominator / denominator;

        float3 kS = F;
        float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
        kD *= 1.0f - metallic;

        float NdotL = max(dot(N, L), 0.0f);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    // 基于图像的光照 (IBL)
    // 漫反射部分 - 使用辐照度贴图
    float3 irradiance = t_irradianceMap.Sample(s_cubeSampler, N).xyz;
    float3 diffuseIBL = irradiance * albedo;

    // 镜面反射部分 - 使用预过滤贴图和BRDF查找表
    const float MAX_REFLECTION_LOD = 6.0f; // 假设预过滤贴图有5个Mip级别 (0-4)
    float3 prefilteredColor = t_prefilterMap.SampleLevel(s_cubeSampler, R,  roughness * MAX_REFLECTION_LOD).xyz;
    float2 brdf = t_brdfLUT.Sample(s_sampler, float2(max(dot(N, V), 0.0f), roughness)).xy;
    float3 specularIBL = prefilteredColor * (F0 * brdf.x + brdf.y);

    // 结合漫反射和镜面IBL
    float3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0f), F0, roughness);
    float3 kD = 1.0f - kS;
    kD *= 1.0f - metallic;

    float3 ambientIBL = (kD * diffuseIBL + specularIBL) * ao;

    // 最终颜色 = 环境光照(IBL) + 直接光照
    float3 color = ambientIBL + Lo;

    // HDR色调映射
    color = color / (color + float3(1.0f, 1.0f, 1.0f));
    // 伽马校正
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

    return float4(color, 1.0f);
}

