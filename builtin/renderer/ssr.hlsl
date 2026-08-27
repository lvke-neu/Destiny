#include "common.hlsl"

struct VertexOut
{
	float4 positionH : SV_POSITION;
	float2 texcoord  : TEXCOORD;
};

VertexOut VS(uint vertexID : SV_VertexID)
{
	VertexOut vOut;
	float2 grid = float2((vertexID << 1) & 2, vertexID & 2);
	float2 xy = grid * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f);
	vOut.positionH = float4(xy, 1.0f, 1.0f);
	vOut.texcoord.x = (xy.x + 1.0f) * 0.5f;
	vOut.texcoord.y = (1.0f - xy.y) * 0.5f;
	return vOut;
}

Texture2D t_sceneColor : register(t0);
Texture2D t_normalW : register(t1);
Texture2D t_positionW : register(t2);
Texture2D t_mra : register(t3);
SamplerState s_sampler : register(s0);

float rand(float2 uv)
{
	return frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453);
}

float4 PS(VertexOut pIn) : SV_TARGET
{
	float4 color = t_sceneColor.Sample(s_sampler, pIn.texcoord);
	float3 normalW = t_normalW.Sample(s_sampler, pIn.texcoord).xyz;
	float4 positionW = t_positionW.Sample(s_sampler, pIn.texcoord);
	float3 mra = t_mra.Sample(s_sampler, pIn.texcoord).xyz;

	if (positionW.w == 0.0f) return color;

	float roughness = mra.y;
	float metallic = mra.x;

	// Skip rough surfaces
	if (roughness > 0.8f) return color;

	float3 viewDir = normalize(positionW.xyz - g_eyePosition.xyz);
	float3 reflectDir = reflect(viewDir, normalW);

	float3 startPos = positionW.xyz;
	float3 rayPos = startPos;
	
	// Transform to View Space for depth comparison
	float3 rayPosV = mul(float4(rayPos, 1.0f), g_view).xyz;
	float3 reflectDirV = mul(reflectDir, (float3x3)g_view);

	// Tweakable parameters
	float stepSize = 0.05f; // Smaller initial step
	int maxSteps = 150;     // More steps
	float thickness = 0.05f; // Surface thickness threshold

	matrix viewProj = mul(g_view, g_proj);

	bool hit = false;
	float3 hitColor = float3(0, 0, 0);

	float texWidth, texHeight;
	t_positionW.GetDimensions(texWidth, texHeight);

	// Add jitter to ray start position to reduce banding artifacts
	float dither = rand(pIn.positionH.xy);
	rayPos += reflectDir * stepSize * dither;
	rayPosV += reflectDirV * stepSize * dither;

	// Basic Linear Raymarch
	[loop]
	for (int i = 0; i < maxSteps; ++i)
	{
		rayPos += reflectDir * stepSize;
		rayPosV += reflectDirV * stepSize;

		// Project to UV
		float4 projPos = mul(float4(rayPos, 1.0f), viewProj);
		projPos.xy /= projPos.w;
		float2 sampleUV = float2(projPos.x * 0.5f + 0.5f, -projPos.y * 0.5f + 0.5f);

		if (sampleUV.x < 0 || sampleUV.x > 1 || sampleUV.y < 0 || sampleUV.y > 1) break;

		// Use Load to avoid interpolation artifacts at depth discontinuities
		int3 loadCoords = int3(sampleUV * float2(texWidth, texHeight), 0);
		float4 samplePosW = t_positionW.Load(loadCoords);

		// Check depth in View Space
		float rayDepth = rayPosV.z;
		float3 samplePosV = mul(float4(samplePosW.xyz, 1.0f), g_view).xyz;
		float sampleDepth = samplePosV.z;

		// If ray is behind surface (rayDepth > sampleDepth in View Space)
		if (rayDepth > sampleDepth)
		{
			// Refinement: Binary Search
			float3 tempRayPos = rayPos - reflectDir * stepSize; // Go back one step
			float3 tempRayPosV = rayPosV - reflectDirV * stepSize;
			float tempStep = stepSize * 0.5f;
			
			[unroll]
			for(int j=0; j<8; ++j)
			{
				float3 midPos = tempRayPos + reflectDir * tempStep;
				float3 midPosV = tempRayPosV + reflectDirV * tempStep;
				
				float4 projPosMid = mul(float4(midPos, 1.0f), viewProj);
				projPosMid.xy /= projPosMid.w;
				float2 midUV = float2(projPosMid.x * 0.5f + 0.5f, -projPosMid.y * 0.5f + 0.5f);
				
				int3 midLoadCoords = int3(midUV * float2(texWidth, texHeight), 0);
				float4 midSamplePosW = t_positionW.Load(midLoadCoords);
				
				float rayDepthMid = midPosV.z;
				float3 midSamplePosV = mul(float4(midSamplePosW.xyz, 1.0f), g_view).xyz;
				float sampleDepthMid = midSamplePosV.z;

				if (rayDepthMid > sampleDepthMid)
				{
					// Still behind, pull back (intersection is in first half)
					// Current range [temp, temp + 2*step]
					// Mid is behind, so hit is between temp and mid
				}
				else
				{
					// In front, push forward (intersection is in second half)
					tempRayPos = midPos;
					tempRayPosV = midPosV;
				}
				tempStep *= 0.5f;
			}
			
			// Final check position
			rayPos = tempRayPos;
			rayPosV = tempRayPosV;
			
			// Re-sample at refined position
			float4 projPosFinal = mul(float4(rayPos, 1.0f), viewProj);
			projPosFinal.xy /= projPosFinal.w;
			float2 finalUV = float2(projPosFinal.x * 0.5f + 0.5f, -projPosFinal.y * 0.5f + 0.5f);
			
			int3 finalLoadCoords = int3(finalUV * float2(texWidth, texHeight), 0);
			float4 finalSamplePosW = t_positionW.Load(finalLoadCoords);
			
			float finalRayDepth = rayPosV.z;
			float3 finalSamplePosV = mul(float4(finalSamplePosW.xyz, 1.0f), g_view).xyz;
			float finalSampleDepth = finalSamplePosV.z;
			
			if (abs(finalRayDepth - finalSampleDepth) < thickness)
			{
				hit = true;
				hitColor = t_sceneColor.SampleLevel(s_sampler, finalUV, 0).rgb;

				// Vignette fade
				float2 edgeFactor = smoothstep(0.0f, 0.2f, finalUV) * smoothstep(0.0f, 0.2f, 1.0f - finalUV);
				float screenEdgeFactor = edgeFactor.x * edgeFactor.y;
				
				hitColor *= screenEdgeFactor; 
			}
			
			break;
		}

		// Increase step size slightly to cover more distance
		stepSize *= 1.02f;
	}

	if (hit)
	{
		float3 F0 = float3(0.04f, 0.04f, 0.04f);
		F0 = lerp(F0, color.rgb, metallic);
		float3 F = fresnelSchlick(max(dot(-viewDir, normalW), 0.0f), F0);

		float factor = (1.0f - roughness);
		return float4(color.rgb + hitColor * F * factor, 1.0f);
	}

	return color;
}
