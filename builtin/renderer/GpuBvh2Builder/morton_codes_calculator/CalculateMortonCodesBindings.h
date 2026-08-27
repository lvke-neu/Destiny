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
#include "../common/RayTracingHelper.hlsli"
#endif


struct MortonCodeCalculatorConstants
{
    uint NumberOfElements;
};

// UAVs
#define MortonCodeCalculatorCalculatorOutputIndices 0
#define MortonCodeCalculatorCalculatorOutputMortonCodes 1
#define MortonCodeCalculatorSceneAABBRegister 2
#define MortonCodeCalculatorInputBufferRegister 3

// CBVs
#define MortonCodeCalculatorConstantsRegister 0

#ifdef HLSL
RWStructuredBuffer<uint> OutputIndicesBuffer : register(u0);
RWStructuredBuffer<uint> OutputMortonCodesBuffer : register(u1);
RWStructuredBuffer<AABB> SceneAABB : register(u2);
cbuffer MortonCodeCalculatorConstants : register(b0)
{
    MortonCodeCalculatorConstants Constants;
}

#define BIT(x) (1 << (x))

AABB GetSceneAABB()
{
    return SceneAABB[0];
}
#endif
