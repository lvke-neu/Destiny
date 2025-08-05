//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#include "../common/RayTracingHlslCompat.h"
#ifdef HLSL
#include "../common/ShaderUtil.hlsli"
#endif


// UAVs
#define OutputBVHRegister 0
#define ScratchBufferRegister 1
#define ChildNodesProcessedBufferRegister 2
#define HierarchyBufferRegister 3
#define AABBParentBufferRegister 4

#define GlobalDescriptorHeapRegister 0
#define GlobalDescriptorHeapRegisterSpace 1

// CBVs
#define InputConstantsRegister 0

#define PREPARE_UPDATE_FLAG 0x1
#define PERFORM_UPDATE_FLAG 0x2
#define ShouldPrepareUpdate UpdateFlags & PREPARE_UPDATE_FLAG
#define ShouldPerformUpdate UpdateFlags & PERFORM_UPDATE_FLAG

#ifdef HLSL
globallycoherent RWByteAddressBuffer outputBVH : register(u0);
RWByteAddressBuffer scratchMemory : register(u1);
RWByteAddressBuffer childNodesProcessedCounter : register(u2);
RWStructuredBuffer<HierarchyNode> hierarchyBuffer : register(u3);
RWStructuredBuffer<uint> aabbParentBuffer : register(u4);
//RWByteAddressBuffer DescriptorHeapBufferTable[] : UAV_REGISTER_SPACE(GlobalDescriptorHeapRegister, GlobalDescriptorHeapRegisterSpace);


cbuffer ConstructHierarchyConstants : register(b0)
{
    uint NumberOfElements;
    uint UpdateFlags;
};
#endif
