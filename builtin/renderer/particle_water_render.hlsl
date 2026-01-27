#include "common.hlsl"

struct Particle
{
    float3 position;
    float3 velocity;
    float3 acceleration;
    float lifetime;
    float maxLifetime;
    float4 color; // Changed to float4 to support alpha
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
    float rotation : TEXCOORD1;
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
    output.rotation = input.index * 123.45f; 

    return output;
}

[maxvertexcount(4)]
void GS(point GS_INPUT input[1], inout TriangleStream<PS_INPUT> outputStream)
{
    GS_INPUT p = input[0];

    // Culling
    if (p.color.a < 0.01f) return;

    float3 forward = normalize(g_eyePosition - p.position.xyz);
    float3 up = float3(0.0f, 1.0f, 0.0f);
    float3 right = normalize(cross(up, forward));
    up = normalize(cross(forward, right));

    float halfSize = p.size * 0.5f;
    
    float c = cos(p.rotation);
    float s = sin(p.rotation);
    float3 rotRight = right * c - up * s;
    float3 rotUp = right * s + up * c;

    float4 v[4];
    v[0] = float4(p.position.xyz + rotRight * halfSize - rotUp * halfSize, 1.0f);
    v[1] = float4(p.position.xyz + rotRight * halfSize + rotUp * halfSize, 1.0f);
    v[2] = float4(p.position.xyz - rotRight * halfSize - rotUp * halfSize, 1.0f);
    v[3] = float4(p.position.xyz - rotRight * halfSize + rotUp * halfSize, 1.0f);

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
    
    // Standard Alpha Blending
    float3 finalColor = texColor.rgb * input.color.rgb;
    float finalAlpha = texColor.a * input.color.a; 
    
    return float4(finalColor, finalAlpha); 
}
