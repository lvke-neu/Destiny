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

struct GS_INPUT
{
    float4 position : SV_POSITION; // World Position
    float4 color : COLOR;
    float size : PSIZE;
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

// Vertex Shader: Fetch particle data and pass to Geometry Shader
GS_INPUT VS(VS_INPUT input)
{
    GS_INPUT output;
    Particle p = particles[input.index];

    // Calculate alpha
    float alpha = saturate(p.lifetime / p.maxLifetime);
    
    // Pass world position directly
    output.position = float4(p.position, 1.0f);
    output.color = float4(p.color, alpha);
    output.size = p.size;

    return output;
}

// Geometry Shader: Expand point to camera-facing quad (Billboard)
[maxvertexcount(4)]
void GS(point GS_INPUT input[1], inout TriangleStream<PS_INPUT> outputStream)
{
    GS_INPUT p = input[0];

    // If particle is dead or invisible, don't emit geometry
    if (p.color.a <= 0.0f) return;

    // Calculate camera basis
    float3 forward = normalize(g_eyePosition - p.position.xyz);
    float3 up = float3(0.0f, 1.0f, 0.0f); // World Up
    float3 right = normalize(cross(up, forward));
    up = normalize(cross(forward, right)); // Re-orthogonalize up

    float halfSize = p.size * 0.5f;

    float4 v[4];
    v[0] = float4(p.position.xyz + right * halfSize - up * halfSize, 1.0f); // Bottom-Right (Wait, let's align UVs)
    v[1] = float4(p.position.xyz + right * halfSize + up * halfSize, 1.0f); // Top-Right
    v[2] = float4(p.position.xyz - right * halfSize - up * halfSize, 1.0f); // Bottom-Left
    v[3] = float4(p.position.xyz - right * halfSize + up * halfSize, 1.0f); // Top-Left

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
    float4 texColor = particleTexture.Sample(samplerState, input.texcoord);
    
    // Simple soft particle circle if no texture is present, or combine with texture
    // For now, assume texture is white dot or fire sprite
    
    float4 finalColor = texColor * input.color;
    
    // Premultiplied alpha or standard blending depending on render state.
    // Assuming standard alpha blending or additive.
    
    return finalColor;
}
