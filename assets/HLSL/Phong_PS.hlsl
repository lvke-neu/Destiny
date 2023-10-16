#include "Common.hlsli"


Texture2D g_Tex : register(t0);
SamplerState g_Sampler : register(s0);
// 像素着色器
float4 PS(VertexOut pIn) : SV_Target
{
    //float4 goAmbientColor = float4(0.5, 0.5, 0.5, 1);
    //float4 camAmbientColor = float4(0.5, 0.5, 0.5, 1);
    //float4 ambient = goAmbientColor * camAmbientColor;


    //float3 lightDir = float3(0, -1,  1);

    //float4 goDiffuseColor = float4(1, 1, 1, 1);
    //float4 camDiffuseColor = float4(1, 1, 1, 1);
    //float4 diffuse = dot(normalize(-lightDir), normalize(pIn.normalW)) * goDiffuseColor * camDiffuseColor;

    //float4 goSpecularColor = float4(1, 1, 1, 1);
    //float4 camSpecularColor = float4(1, 1, 1, 1);
    //float3 reflecV = reflect(normalize(lightDir), normalize(pIn.normalW));
    //float4 specular = pow(max(dot(reflecV, normalize(float3(-1, 1,0) - pIn.posW)), 0.0f), 128) * goSpecularColor * camSpecularColor;

    //return float4(1,1,1,1) * (ambient + diffuse + specular);
    return g_Tex.Sample(g_Sampler, pIn.tex);
}
