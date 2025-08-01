#include "common.hlsl"

struct Particle
{
    float3 position;
    float3 velocity;
    float3 acceleration;
    float lifetime;
    float maxLifetime;
    float3 color;
    float size;
};

struct VS_INPUT
{
    uint index : SV_VertexID;
};


struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

StructuredBuffer<Particle> particles : register(t0);
Texture2D particleTexture : register(t1);
SamplerState samplerState : register(s0);

// 顶点着色器 - 从粒子缓冲区获取数据
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    Particle p = particles[input.index];

    // 计算 alpha 值(生命周期结束时逐渐消失)
    float alpha = saturate(p.lifetime / p.maxLifetime);
  
    output.position = mul(mul(float4(p.position, 1.0f), g_view), g_proj);
    output.color = float4(p.color, alpha);

    return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
    return input.color;
}
