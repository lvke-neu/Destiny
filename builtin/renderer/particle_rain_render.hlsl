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

struct VS_INPUT
{
    uint index : SV_VertexID;
};

struct GS_INPUT
{
    float4 position : SV_POSITION; 
    float4 color : COLOR;
    float size : PSIZE;
    float3 velocity : TEXCOORD1;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

StructuredBuffer<Particle> particles : register(t0);
Texture2D particleTexture : register(t1);
SamplerState samplerState : register(s0);

GS_INPUT VS(VS_INPUT input)
{
    GS_INPUT output;
    Particle p = particles[input.index];

    output.position = float4(p.position, 1.0f);
    output.color = p.color; 
    output.size = p.size;
    output.velocity = p.velocity;

    return output;
}

[maxvertexcount(4)]
void GS(point GS_INPUT input[1], inout TriangleStream<PS_INPUT> outputStream)
{
    GS_INPUT p = input[0];

    // Rain Streak Alignment
    // Instead of camera facing, rain streaks align with velocity
    
    float3 velDir = normalize(p.velocity); // Usually (0, -1, 0)
    float3 viewDir = normalize(g_eyePosition - p.position.xyz);
    
    // Cross product to find side vector (billboard width)
    float3 side = normalize(cross(viewDir, velDir));
    
    // Length of streak
    float length = 1.0f; // Could be uniform or based on speed
    float width = p.size;
    
    float3 pTop = p.position.xyz - velDir * length * 0.5f;
    float3 pBottom = p.position.xyz + velDir * length * 0.5f;
    
    float4 v[4];
    v[0] = float4(pBottom + side * width, 1.0f); // Bottom Right
    v[1] = float4(pTop + side * width, 1.0f);    // Top Right
    v[2] = float4(pBottom - side * width, 1.0f); // Bottom Left
    v[3] = float4(pTop - side * width, 1.0f);    // Top Left

    float2 texCoords[4] = 
    {
        float2(1.0f, 1.0f),
        float2(1.0f, 0.0f),
        float2(0.0f, 1.0f),
        float2(0.0f, 0.0f)
    };

    PS_INPUT outV;
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        outV.position = mul(mul(v[i], g_view), g_proj);
        outV.color = p.color; 
        outV.texcoord = texCoords[i];
        outputStream.Append(outV);
    }
}

float4 PS(PS_INPUT input) : SV_Target
{
    // Simple gradient for streak or texture
    // For rain, often just a white gradient is enough
    
    float alpha = input.color.a;
    
    // Fade at ends (Top and Bottom of UV)
    // UV.y goes 0 (Top) to 1 (Bottom)
    // Rain drop is usually brighter at bottom (head) and fades at tail (top)
    // Or uniform streak. Let's do simple fade at tips.
    float fade = sin(input.texcoord.y * 3.14159);
    
    return float4(input.color.rgb, alpha * fade);
}
