#include "common.hlsl"

struct Particle
{
    float3 position;
    float3 velocity;
    float3 acceleration;
    float lifetime;
    float maxLifetime;
    float4 color;
    float size;
};

cbuffer cbParticleUpdate : register(b0)
{
    float deltaTime;
    float3 emitterPosition;
    float totalTime;
    float3 rainAreaSize; // Width, Height (Start Y), Depth
    float fallSpeed;
    float dropLength;
    float2 padding;
};

RWStructuredBuffer<Particle> particles : register(u0);

float rand(float2 co)
{
    return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}

[numthreads(64, 1, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint index = dispatchThreadID.x;
    Particle p = particles[index];

    // Physics
    p.position += p.velocity * deltaTime;
    
    // Check if below ground or out of bounds (Simple wrapping for infinite rain)
    // Emitter position is center of rain box
    float bottomY = emitterPosition.y - rainAreaSize.y * 0.5f;
    
    // Respawn logic (Init or wrapped)
    if (p.position.y < bottomY || p.lifetime <= 0.0f)
    {
        float seed = index + totalTime * 10.0f;
        
        // Random position in box
        float randX = (rand(float2(seed, 0.1)) - 0.5f) * rainAreaSize.x;
        float randZ = (rand(float2(seed, 0.2)) - 0.5f) * rainAreaSize.z;
        float randY = (rand(float2(seed, 0.3)) - 0.5f) * rainAreaSize.y; // Initial spread
        
        // If respawning, put at top
        if (p.lifetime <= 0.0f) {
             randY = rainAreaSize.y * 0.5f;
        }

        p.position = emitterPosition + float3(randX, randY, randZ);
        
        // Velocity (Downwards with slight wind variation?)
        float speedVar = 1.0f + rand(float2(seed, 0.4)) * 0.2f;
        p.velocity = float3(0.0f, -fallSpeed * speedVar, 0.0f);
        
        p.acceleration = float3(0.0f, 0.0f, 0.0f);
        
        p.maxLifetime = 100.0f; // Long life, managed by wrapping
        p.lifetime = p.maxLifetime;
        
        // Size (Width, Length packed?) or just uniform size
        // Rain is thin and long. We use size for width, and stretch in GS.
        p.size = 0.05f + rand(float2(seed, 0.5)) * 0.02f;
        
        p.color = float4(0.8f, 0.9f, 1.0f, 0.6f); // Semi-transparent blue-white
    }
    
    particles[index] = p;
}
