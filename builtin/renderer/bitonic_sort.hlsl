RWBuffer<uint> u_sortedBufferIn : register(u0);
RWBuffer<uint> u_sortedBufferOut : register(u1);

//SV_DispatchThreadID 
//SV_GroupID
//SV_GroupThreadID

[numthreads(1, 1, 1)]
void CS(uint3 groupThreadID : SV_GroupThreadID)
{
	for (uint i = 0; i < 9; i++)
	{
		for (uint j = 0; j < 9 - i; j++)
		{
			if (u_sortedBufferIn[j] > u_sortedBufferIn[j + 1])
			{
				uint tmp = u_sortedBufferIn[j];
				u_sortedBufferIn[j] = u_sortedBufferIn[j + 1];
				u_sortedBufferIn[j + 1] = tmp;
			}
		}
	}
}