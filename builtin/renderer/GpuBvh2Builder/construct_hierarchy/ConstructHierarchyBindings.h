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

#include "../common/RaytracingHlslCompat.h"
#ifdef HLSL
#include "../common/ShaderUtil.hlsli"
#endif

struct InputConstants
{
    uint NumberOfElements;
};

// UAVs
#define MortonCodesBufferRegister 0
#define HierarchyBufferRegister 1

#define GlobalDescriptorHeapRegister 0
#define GlobalDescriptorHeapRegisterSpace 1

// CBVs
#define InputConstantsRegister 0

#ifdef HLSL
RWStructuredBuffer<uint> mortonCodes : register(u0);
RWStructuredBuffer<HierarchyNode> hierarchyBuffer : register(u1);

cbuffer ConstructHierarchyConstants : register(b0)
{
    InputConstants Constants;
};
#endif
