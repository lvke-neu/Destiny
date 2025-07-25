struct ClusterBound
{
	uint x;
	uint y;
	uint z;
	uint w;
};


RWStructuredBuffer<ClusterBound> u_clusterBoundsIn : register(u0);
RWStructuredBuffer<ClusterBound> u_clusterBoundsOut : register(u1);

//SV_DispatchThreadID 
//SV_GroupID
//SV_GroupThreadID

[numthreads(16, 16, 1)]
void CS(uint3 groupThreadID : SV_GroupThreadID)
{
	uint index = groupThreadID.x + groupThreadID.y * 16;
	u_clusterBoundsOut[index] = u_clusterBoundsIn[index];
}