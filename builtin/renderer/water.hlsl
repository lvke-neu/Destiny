#include "common.hlsl"

cbuffer cbPerObject : register(b0)
{
	matrix u_worldMatrix;
    matrix u_worldInvTransposeMatrix;
    float4 u_waveParams1;    // 波浪参数1: amplitude, frequency, speed, steepness
    float4 u_waveParams2;    // 波浪参数2: amplitude, frequency, speed, steepness
    float4 u_waveParams3;    // 波浪参数3: amplitude, frequency, speed, steepness
    float4 u_waterColor;     // 水的颜色
    float  u_time;           // 当前时间
    float  u_waveScale;      // 波浪缩放
    float  u_waterLevel;     // 水面高度
}

struct VertexIn
{
	float3 positionL : POSITION;
	float2 texcoord  : TEXCOORD;
};

struct VertexOut
{
	float4 positionH : SV_POSITION;
	float4 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 texcoord  : TEXCOORD;
};

// 计算波浪高度和法线
float3 CalculateWaveDisplacement(float3 position, float2 texcoord, out float3 normal)
{
    // 初始化高度和法线
    float height = 0.0;
    float dx = 0.0;
    float dz = 0.0;

    // 波浪1
    float2 waveDir1 = normalize(float2(1.0, 0.5));
    float wave1Phase = dot(waveDir1, texcoord * u_waveScale) * u_waveParams1.y + u_time * u_waveParams1.z;
    float wave1Height = u_waveParams1.x * sin(wave1Phase);
    float wave1Steepness = u_waveParams1.w;

    height += wave1Height;
    dx += wave1Height * waveDir1.x * wave1Steepness * cos(wave1Phase);
    dz += wave1Height * waveDir1.y * wave1Steepness * cos(wave1Phase);

    // 波浪2
    float2 waveDir2 = normalize(float2(-0.5, 1.0));
    float wave2Phase = dot(waveDir2, texcoord * u_waveScale) * u_waveParams2.y + u_time * u_waveParams2.z;
    float wave2Height = u_waveParams2.x * sin(wave2Phase);
    float wave2Steepness = u_waveParams2.w;

    height += wave2Height;
    dx += wave2Height * waveDir2.x * wave2Steepness * cos(wave2Phase);
    dz += wave2Height * waveDir2.y * wave2Steepness * cos(wave2Phase);

    // 波浪3 (可选，用于更复杂的波浪)
    float2 waveDir3 = normalize(float2(0.7, -0.7));
    float wave3Phase = dot(waveDir3, texcoord * u_waveScale) * u_waveParams3.y + u_time * u_waveParams3.z;
    float wave3Height = u_waveParams3.x * sin(wave3Phase);
    float wave3Steepness = u_waveParams3.w;

    height += wave3Height;
    dx += wave3Height * waveDir3.x * wave3Steepness * cos(wave3Phase);
    dz += wave3Height * waveDir3.y * wave3Steepness * cos(wave3Phase);

    // 计算法线 (使用高度场的导数)
    normal = normalize(float3(-dx, 1.0, -dz));

    // 返回位移后的位置
    return float3(position.x, height + u_waterLevel, position.z);
}

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;

    // 计算波浪位移和法线
    float3 normal;
    float3 displacedPos = CalculateWaveDisplacement(vIn.positionL, vIn.texcoord, normal);

    vOut.positionW = mul(float4(displacedPos, 1.0), u_worldMatrix);
	vOut.positionH = mul(mul(vOut.positionW, g_view), g_proj);
    vOut.normalW = mul(normal, (float3x3) u_worldInvTransposeMatrix);
	vOut.texcoord = vIn.texcoord;
   

	return vOut;
}

Texture2D t_albedo : register(t0);
Texture2D t_normal : register(t1);
Texture2D t_metallic : register(t2);
Texture2D t_roughness : register(t3);
Texture2D t_ao : register(t4);
Texture2D t_underwater : register(t5);
SamplerState s_sampler : register(s0);

float4 PS(VertexOut pIn) : SV_Target
{
    float3 albedo = pow(u_waterColor.xyz, float3(2.2f, 2.2f, 2.2f));
    float metallic = t_metallic.Sample(s_sampler, pIn.texcoord).x;
    float roughness = t_roughness.Sample(s_sampler, pIn.texcoord).x;
    float ao = t_ao.Sample(s_sampler, pIn.texcoord).x;

    //getNormalFromMap---------
    float3 tangentNormal = t_normal.Sample(s_sampler, pIn.texcoord).xyz * 2.0f - 1.0f;
    float3 Q1 = ddx(pIn.positionW.xyz);
    float3 Q2 = ddy(pIn.positionW.xyz);
    float2 st1 = ddx(pIn.texcoord);
    float2 st2 = ddy(pIn.texcoord);

    float3 N_ = normalize(pIn.normalW);
    float3 T = normalize(Q1 * st2.y - Q2 * st1.y);
    float3 B = -normalize(cross(N_, T));

    float3x3 TBN = float3x3(T, B, N_);

    //-------------------------

    float3 N = normalize(mul(tangentNormal, TBN));
    float3 V = normalize(g_eyePosition.xyz - pIn.positionW.xyz);

    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, albedo, metallic);

    float3 Lo = float3(0.0f, 0.0f, 0.0f);

    //radiance
    for (int i = 0; i < g_directionLightCount; i++)
    {
        float3 L = normalize(-g_directionLights[i].lightDirection);
        float3 H = normalize(V + L);

        float3 radiance = g_directionLights[i].lightColor.xyz * g_directionLights[i].lightIntensity;

        // cook-torrance brdf
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

            Lo += (kD * albedo / PI + specular /*+ t_environment.Sample(s_sampler, reflect(-V, N_)).xyz * kS*/) * radiance * NdotL;

    }
   for (int j = 0; j < g_pointLightCount; j++)
   {
       float3 L = normalize(g_pointLights[j].lightPosition - pIn.positionW.xyz);
       float3 H = normalize(V + L);

       float distance = length(g_pointLights[j].lightPosition - pIn.positionW.xyz);
       float attenuation = 1.0 / (distance * distance);
       float3 radiance = g_pointLights[j].lightColor.xyz * g_pointLights[j].lightIntensity * attenuation;

       // cook-torrance brdf
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
        Lo += (kD * albedo / PI + specular /*+ t_environment.Sample(s_sampler, reflect(-V, N_)).xyz * kS*/) * radiance * NdotL;
   }

    float3 ambient = float3(0.03f, 0.03f, 0.03f) * albedo * ao;
    float3 color = ambient + Lo;

    color = color / (color + float3(1.0f, 1.0f, 1.0f));
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

    //法线扰动模拟折射
    float2 refractOffset = pIn.normalW.xy * 0.03;
    float3 underwaterColor = t_underwater.Sample(s_sampler, pIn.texcoord + refractOffset).xyz;
    float3 finalColor = lerp(underwaterColor, color.xyz, u_waterColor.a);

    return float4(finalColor, u_waterColor.a);
}
