RWByteAddressBuffer g_IndirectArgsBuffer : register(u0);

//SV_DispatchThreadID 
//SV_GroupID
//SV_GroupThreadID

[numthreads(2, 1, 1)]
void CS(uint3 groupThreadID : SV_GroupThreadID)
{
	g_IndirectArgsBuffer.Store3(groupThreadID.x, uint3(1, 2, 3));
}