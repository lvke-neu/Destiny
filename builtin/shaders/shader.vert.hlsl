struct UBO {
    float4x4 model;
    float4x4 view;
    float4x4 proj;
};

cbuffer ubo : register(b0) {
    UBO ubo;
};

struct VSInput {
    float3 Pos : POSITION;
    float3 Color : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

struct VSOutput {
    float4 Pos : SV_POSITION;
    float3 Color : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

VSOutput main(VSInput input) {
    VSOutput output;
    // 矩阵乘法顺序：Proj * View * Model * Vector
    // 在 HLSL 中使用 mul() 函数
    float4 pos = float4(input.Pos, 1.0);
    pos = mul(ubo.model, pos);
    pos = mul(ubo.view, pos);
    pos = mul(ubo.proj, pos);
    
    output.Pos = pos;
    output.Color = input.Color;
    output.TexCoord = input.TexCoord;
    return output;
}
