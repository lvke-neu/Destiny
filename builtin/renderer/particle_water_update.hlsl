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
    float3 gravity; // Replaced padding with controllable gravity
    float spreadRadius; // New parameters start here
    float splashHeight;
    float particleMinSize;
    float particleMaxSize;
    float particleMinLife;
    float particleMaxLife;
    float2 padding2; // Alignment padding
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

    p.lifetime -= deltaTime;

    if (p.lifetime <= 0.0f)
    {
        float seed = index + totalTime * 10.0f;

        // Reset to emitter position with slight random offset
        float offsetAngle = rand(float2(seed, 0.05)) * 6.28;
        float offsetR = rand(float2(seed, 0.06)) * (spreadRadius * 0.1f); // Base radius relative to spread
        p.position = emitterPosition + float3(cos(offsetAngle)*offsetR, 0, sin(offsetAngle)*offsetR);
        
        // Splash Velocity
        float angle = rand(float2(seed, 0.1)) * 6.28318;
        float currentSpread = spreadRadius * (0.5f + rand(float2(seed, 0.2)) * 0.5f); 
        float currentUpward = splashHeight * (0.8f + rand(float2(seed, 0.3)) * 0.4f); 
        
        p.velocity = float3(cos(angle) * currentSpread, currentUpward, sin(angle) * currentSpread);

        // Gravity
        p.acceleration = gravity;

        p.maxLifetime = particleMinLife + rand(float2(seed, 0.4)) * (particleMaxLife - particleMinLife);
        p.lifetime = p.maxLifetime;

        p.size = particleMinSize + rand(float2(seed, 0.5)) * (particleMaxSize - particleMinSize);
        
        // Base Color
        p.color = float4(0.8f, 0.9f, 1.0f, 0.0f);
    }
    else
    {
        // Physics update
        p.velocity += p.acceleration * deltaTime;
        p.position += p.velocity * deltaTime;
        
        // Ground collision 
        if (p.position.y < emitterPosition.y)
        {
             p.position.y = emitterPosition.y;
             p.velocity.y = -p.velocity.y * 0.3f; // Bounce
             p.velocity.xz *= 0.8f; 
        }
    }

    // Alpha/Color logic
    float lifeRatio = p.lifetime / p.maxLifetime;
    
    float alpha = smoothstep(0.0, 0.1, 1.0 - lifeRatio) * smoothstep(0.0, 0.2, lifeRatio);
    alpha *= 0.8f; 
    
    p.color = float4(0.9f, 0.95f, 1.0f, alpha); 

    particles[index] = p;
}
