Texture2D t_texture0 : register(t0);
Texture2D t_texture1 : register(t1);

RWTexture2D<unorm float4> u_texture : register(u0);

//SV_DispatchThreadID 
//SV_GroupID
//SV_GroupThreadID

[numthreads(16, 16, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	u_texture[dispatchThreadID.xy] = (unorm float4)(t_texture0[dispatchThreadID.xy] * t_texture1[dispatchThreadID.xy]);
}