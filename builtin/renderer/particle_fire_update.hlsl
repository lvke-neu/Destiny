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
    float totalTime; // Added for randomness
    float3 padding;
};

RWStructuredBuffer<Particle> particles : register(u0);

// Random function
float rand(float2 co)
{
    return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}

[numthreads(64, 1, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint index = dispatchThreadID.x;
    Particle p = particles[index];

    // Update lifetime
    p.lifetime -= deltaTime;

    // Respawn logic
    if (p.lifetime <= 0.0f)
    {
        // Use totalTime to vary the random seed over time
        float seed = index + totalTime * 10.0f;

        // Reset position at emitter with small sphere offset
        float3 randomOffset = float3(
            rand(float2(seed, 0.1)) * 2.0 - 1.0,
            rand(float2(seed, 0.2)) * 2.0 - 1.0,
            rand(float2(seed, 0.3)) * 2.0 - 1.0
        );
        p.position = emitterPosition + normalize(randomOffset) * (rand(float2(seed, 0.9)) * 0.2); // 0.2 radius sphere

        // Reset velocity (Upwards cone)
        p.velocity = float3(
            rand(float2(seed, 0.4)) * 1.0 - 0.5, // -0.5 to 0.5
            1.5f + rand(float2(seed, 0.5)) * 1.5, // 1.5 to 3.0
            rand(float2(seed, 0.6)) * 1.0 - 0.5  // -0.5 to 0.5
        );

        // Reset acceleration (Buoyancy)
        p.acceleration = float3(0.0f, 1.0f, 0.0f);

        // Reset lifetime
        p.maxLifetime = 1.0f + rand(float2(seed, 0.7)) * 1.0f;
        p.lifetime = p.maxLifetime;

        // Reset size
        p.size = 0.2f + rand(float2(seed, 0.8)) * 0.3f;
        
        // Initial color (Yellow core)
        p.color = float3(1.0f, 0.8f, 0.2f); 
    }
    else
    {
        // Physics update
        p.velocity += p.acceleration * deltaTime;
        p.position += p.velocity * deltaTime;

        // Turbulence (Simulate wind/heat distortion)
        float3 noise = float3(
            rand(float2(p.position.y, totalTime)) * 2.0 - 1.0,
            0.0f,
            rand(float2(p.position.z, totalTime)) * 2.0 - 1.0
        );
        p.position += noise * deltaTime * 0.5f;
    }

    // Color over lifetime logic
    float lifeRatio = p.lifetime / p.maxLifetime;
    
    // Gradient: Yellow -> Orange -> Red -> Dark Smoke
    if (lifeRatio > 0.8)
    {
        p.color = lerp(float3(1.0, 0.5, 0.0), float3(1.0, 1.0, 0.5), (lifeRatio - 0.8) * 5.0);
    }
    else if (lifeRatio > 0.4)
    {
        p.color = lerp(float3(0.8, 0.1, 0.0), float3(1.0, 0.5, 0.0), (lifeRatio - 0.4) * 2.5);
    }
    else
    {
        p.color = lerp(float3(0.1, 0.1, 0.1), float3(0.8, 0.1, 0.0), lifeRatio * 2.5);
    }

    // Size over lifetime (Grow then shrink)
    if (lifeRatio > 0.5)
        p.size += deltaTime * 0.2f;
    else
        p.size -= deltaTime * 0.1f;

    particles[index] = p;
}
