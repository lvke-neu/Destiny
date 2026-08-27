#include "common.hlsl"

cbuffer cbPerObject : register(b0)
{
	matrix u_worldMatrix;
	float exposure;
}

struct VertexIn
{
	float3 positionL : POSITION;
};

struct VertexOut
{
	float4 positionH : SV_POSITION;
	float3 positionL : POSITIONL;
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	float4x4 view = g_view;
	view[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
	vOut.positionH = mul(mul(mul(float4(vIn.positionL, 1.0f), u_worldMatrix), view), g_proj).xyww;
	vOut.positionL = vIn.positionL;

	return vOut;
}


// Reinhard色调映射
float3 ToneMapReinhard(float3 color)
{
	return color / (color + 1.0f);
}

// 曝光色调映射
float3 ToneMapExposure(float3 color, float exposure)
{
	return 1.0f - exp(-color * exposure);
}

// 伽马校正
float3 GammaCorrect(float3 color, float gamma = 2.2f)
{
	return pow(color, 1.0f / gamma);
}

TextureCube t_cube : register(t0);
SamplerState s_sampler : register(s0);
float4 PS(VertexOut pIn) : SV_Target
{
	// 采样HDR天空盒
	float3 hdrColor = t_cube.Sample(s_sampler, pIn.positionL).rgb;

	// 应用色调映射将HDR颜色转换为LDR
	float3 ldrColor = ToneMapExposure(hdrColor, exposure);

	// 应用伽马校正
	float3 finalColor = GammaCorrect(ldrColor);

	return float4(finalColor, 1.0f);
}

