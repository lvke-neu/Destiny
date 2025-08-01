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

cbuffer cbParticleUpdate : register(b0)
{
    float deltaTime;
    float3 emitterPosition;
};

RWStructuredBuffer<Particle> particles : register(u0);

// 随机数生成函数
float rand(float2 co)
{
    return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}

[numthreads(64, 1, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint index = dispatchThreadID.x;
    Particle p = particles[index];

    // 更新生命周期
    p.lifetime -= deltaTime;

    // 如果粒子已过期，重置它
    if (p.lifetime <= 0.0f)
    {
        // 重置到发射器位置附近
        p.position = emitterPosition + float3(
            rand(float2(index, 0.1)) * 0.2f - 0.1f,
            rand(float2(index, 0.2)) * 0.2f - 0.1f,
            rand(float2(index, 0.3)) * 0.2f - 0.1f
            );

        // 随机初始速度(向上为主，模拟火焰)
        p.velocity = float3(
            rand(float2(index, 0.4)) * 0.5f - 0.25f,
            1.0f + rand(float2(index, 0.5)) * 0.5f,
            rand(float2(index, 0.6)) * 0.5f - 0.25f
            );

        // 加速度
        p.acceleration = float3(0.0f, 0.5f, 0.0f);

        // 重置生命周期
        p.maxLifetime = 0.5f + rand(float2(index, 0.7)) * 1.5f;
        p.lifetime = p.maxLifetime;

        // 随机大小
        p.size = 0.1f + rand(float2(index, 0.8)) * 0.4f;
    }
    else
    {
        // 更新粒子物理
        p.velocity += p.acceleration * deltaTime;
        p.position += p.velocity * deltaTime;

        // 添加一些随机扰动，使火焰更自然
        p.position.x += (rand(float2(p.lifetime, index)) * 2.0f - 1.0f) * deltaTime * 0.3f;
        p.position.z += (rand(float2(index, p.lifetime)) * 2.0f - 1.0f) * deltaTime * 0.3f;
    }

    // 更新颜色 - 随生命周期变化，模拟火焰从黄色变为红色再到透明
    float lifeRatio = p.lifetime / p.maxLifetime;

    if (lifeRatio > 0.7f)
    {
        // 初期 - 明亮的黄色
        p.color = float3(1.0f, 1.0f, 0.3f);
    }
    else if (lifeRatio > 0.3f)
    {
        // 中期 - 橙色到红色过渡
        p.color = float3(1.0f, lifeRatio * 2.0f, 0.0f);
    }
    else
    {
        // 末期 - 红色并逐渐透明
        p.color = float3(1.0f, 0.0f, 0.0f) * lifeRatio * 3.0f;
    }

    // 粒子随时间变大
    p.size += deltaTime * 0.1f;

    // 将更新后的粒子写回缓冲区
    particles[index] = p;
}
