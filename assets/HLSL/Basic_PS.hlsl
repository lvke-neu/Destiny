#include "Common.hlsli"

static const float SMAP_SIZE = 2048.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;
float CalcShadowFactor(SamplerComparisonState samShadow,
	Texture2D shadowMap,
	float4 shadowPosH)
{
	// 透视除法
	shadowPosH.xyz /= shadowPosH.w;

	// NDC空间的深度值
	float depth = shadowPosH.z;

	// 纹素在纹理坐标下的宽高
	const float dx = SMAP_DX;

	float percentLit = 0.0f;
	const float2 offsets[9] =
	{
		float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
	};

	// samShadow为compareValue <= sampleValue时为1.0f(反之为0.0f), 对相邻四个纹素进行采样比较
	// 并根据采样点位置进行双线性插值
	// float result0 = depth <= s0;  // .s0      .s1          
	// float result1 = depth <= s1;
	// float result2 = depth <= s2;  //     .depth
	// float result3 = depth <= s3;  // .s2      .s3
	// float result = BilinearLerp(result0, result1, result2, result3, a, b);  // a b为算出的插值相对位置                           
	[unroll]
	for (int i = 0; i < 9; ++i)
	{
		percentLit += shadowMap.SampleCmpLevelZero(samShadow,
			shadowPosH.xy + offsets[i], depth).r;
	}

	return percentLit /= 9.0f;
}
	
float4 PS(VertexOut pIn) : SV_Target
{
	//float4 color = g_ambientTexture.Sample(g_ambientSampler, pIn.texcoord);
	//color.a = 0.5f;
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;

	float ambient = 1.0f;
	float diffuse = 1.0f * dot(normalize(-g_directLightDirection.xyz), normalize(pIn.normalW));
	float3 v = reflect(g_directLightDirection.xyz, pIn.normalW);
	float specular = 1.0f * pow(max(dot(normalize(v), normalize(g_eyePos.xyz - pIn.positionW.xyz)), 0.0f), 32.0f);
	
	if (g_useColor.x == 1.0f)
	{
		ambientColor = ambient * g_ambientColor;
		diffuseColor = diffuse * g_diffuseColor;
		specularColor = specular * g_specularColor;
	}
	else
	{
		ambientColor = ambient * g_ambientTexture.Sample(g_ambientSampler, pIn.texcoord);
		diffuseColor = diffuse * g_diffuseTexture.Sample(g_diffuseSampler, pIn.texcoord);
		specularColor = specular * g_specularTexture.Sample(g_specularSampler, pIn.texcoord);
	}

	float shadow = CalcShadowFactor(g_shadowMapSampler, g_shadowMapTexture, pIn.shadowMapPositionH);
	//ambientColor = ambient * g_ambientTexture.Sample(g_ambientSampler, pIn.texcoord) * g_ambientColor;;
	//diffuseColor = diffuse * g_diffuseTexture.Sample(g_diffuseSampler, pIn.texcoord) * g_diffuseColor;
	//specularColor = specular * g_specularTexture.Sample(g_specularSampler, pIn.texcoord) * g_specularColor;
	
	return shadow * (ambientColor + diffuseColor + specularColor);
}
