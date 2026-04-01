struct PSInput {
    float4 Pos : SV_POSITION;
    float3 Color : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

Texture2D tex : register(t1);
SamplerState sam : register(s1);

float4 main(PSInput input) : SV_TARGET {
    return tex.Sample(sam, input.TexCoord);
}
